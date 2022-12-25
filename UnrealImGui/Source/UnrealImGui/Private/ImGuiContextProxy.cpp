// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ImGuiContextProxy.h"

#include "ImGuiDelegatesContainer.h"
#include "ThirdPartyBuildImGui.h"
#include "ThirdPartyBuildNetImgui.h"
#include "ImGuiInteroperability.h"
#include "Utilities/Arrays.h"
#include "VersionCompatibility.h"

#include <GenericPlatform/GenericPlatformFile.h>
#include <Misc/Paths.h>

//@Begin KLMod
#include "Engine/World.h"
// End KLMod

static constexpr float DEFAULT_CANVAS_WIDTH = 3840.f;
static constexpr float DEFAULT_CANVAS_HEIGHT = 2160.f;


namespace
{
	FString GetSaveDirectory()
	{
#if ENGINE_COMPATIBILITY_LEGACY_SAVED_DIR
		const FString SavedDir = FPaths::GameSavedDir();
#else
		const FString SavedDir = FPaths::ProjectSavedDir();
#endif

		FString Directory = FPaths::Combine(*SavedDir, TEXT("ImGui"));

		// Make sure that directory is created.
		IPlatformFile::GetPlatformPhysical().CreateDirectory(*Directory);

		return Directory;
	}

	FString GetIniFile(const FString& Name)
	{
		static FString SaveDirectory = GetSaveDirectory();
		return FPaths::Combine(SaveDirectory, Name + TEXT(".ini"));
	}

	struct FGuardCurrentContext
	{
		FGuardCurrentContext()
			: OldContext(ImGui::GetCurrentContext())
		{
		}

		~FGuardCurrentContext()
		{
			if (bRestore)
			{
				ImGui::SetCurrentContext(OldContext);
			}
		}

		FGuardCurrentContext(FGuardCurrentContext&& Other)
			: OldContext(MoveTemp(Other.OldContext))
		{
			Other.bRestore = false;
		}

		FGuardCurrentContext& operator=(FGuardCurrentContext&&) = delete;

		FGuardCurrentContext(const FGuardCurrentContext&) = delete;
		FGuardCurrentContext& operator=(const FGuardCurrentContext&) = delete;

	private:

		ImGuiContext* OldContext = nullptr;
		bool bRestore = true;
	};
}

FImGuiContextProxy::FImGuiContextProxy(const FString& InName, int32 InContextIndex, ImFontAtlas* InFontAtlas, float InDPIScale, const UWorld& _World)
	: Name(InName)
	, ContextIndex(InContextIndex)
	, IniFilename(TCHAR_TO_ANSI(*GetIniFile(InName)))
    , mWorld(&_World)
{
	// Create context.
	Context = ImGui::CreateContext(InFontAtlas);

	// Set this context in ImGui for initialization (any allocations will be tracked in this context).
	SetAsCurrent();

	// Start initialization.
	ImGuiIO& IO = ImGui::GetIO();

	// Set session data storage.
	IO.IniFilename = IniFilename.c_str();

	// Start with the default canvas size.
	ResetDisplaySize();
    //@BeginKLMOD: use float instead of double
    IO.DisplaySize = {static_cast<float>(DisplaySize.X), static_cast<float>(DisplaySize.Y) };

	// Set the initial DPI scale.
	SetDPIScale(InDPIScale);

	// Initialize key mapping, so context can correctly interpret input state.
	ImGuiInterops::SetUnrealKeyMap(IO);

	// Begin frame to complete context initialization (this is to avoid problems with other systems calling to ImGui
	// during startup).
	BeginFrame();
}

FImGuiContextProxy::~FImGuiContextProxy()
{
	if (Context)
	{
		// It seems that to properly shutdown context we need to set it as the current one (at least in this framework
		// version), even though we can pass it to the destroy function.
		SetAsCurrent();

		// Save context data and destroy.
		ImGui::DestroyContext(Context);
	}
}

void FImGuiContextProxy::ResetDisplaySize()
{
	DisplaySize = { DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT };
}

void FImGuiContextProxy::SetDPIScale(float Scale)
{
	if (DPIScale != Scale)
	{
		DPIScale = Scale;

		ImGuiStyle NewStyle = ImGuiStyle();
		NewStyle.ScaleAllSizes(Scale);

		FGuardCurrentContext GuardContext;
		SetAsCurrent();
		ImGui::GetStyle() = MoveTemp(NewStyle);
	}
}

void FImGuiContextProxy::DrawEarlyDebug()
{
	if (bIsFrameStarted && !bIsDrawEarlyDebugCalled)
	{
		bIsDrawEarlyDebugCalled = true;
		if( NetImGuiCanDrawProxy(this) )
		{			
			SetAsCurrent();
			
			// Delegates called in order specified in FImGuiDelegates.
			BroadcastMultiContextEarlyDebug();
			BroadcastWorldEarlyDebug();
		}

		if( NetImGuiSetupDrawRemote(this) )
		{
			BroadcastMultiContextEarlyDebug();
			BroadcastWorldEarlyDebug();
		}
	}
}

void FImGuiContextProxy::DrawDebug()
{
	if (bIsFrameStarted && !bIsDrawDebugCalled)
	{
		bIsDrawDebugCalled = true;

		// Make sure that early debug is always called first to guarantee order specified in FImGuiDelegates.
		DrawEarlyDebug();

		if (NetImGuiCanDrawProxy(this))
		{			
			SetAsCurrent();

			// Delegates called in order specified in FImGuiDelegates.
			BroadcastWorldDebug();
			BroadcastMultiContextDebug();
		}

		if (NetImGuiSetupDrawRemote(this))
		{
			BroadcastWorldDebug();
			BroadcastMultiContextDebug();
		}
	}
}

void FImGuiContextProxy::Tick(float DeltaSeconds)
{
    //@Begin KLMod: Add profiling
    QUICK_SCOPE_CYCLE_COUNTER(STAT_ImGuiContextProxy_Tick);

	// Making sure that we tick only once per frame.
	if (LastFrameNumber < GFrameNumber)
	{
		LastFrameNumber = GFrameNumber;

		if (bIsFrameStarted)
		{
			// Make sure that draw events are called before the end of the frame.
			DrawDebug();

			// Ending frame will produce render output that we capture and store for later use. This also puts context to
			// state in which it does not allow to draw controls, so we want to immediately start a new frame.
			EndFrame();
		}

		// Update context information (some data need to be collected before starting a new frame while some other data
		// may need to be collected after).
		bHasActiveItem = ImGui::IsAnyItemActive();
		MouseCursor = ImGuiInterops::ToSlateMouseCursor(ImGui::GetMouseCursor());

		// Begin a new frame and set the context back to a state in which it allows to draw controls.
		BeginFrame(DeltaSeconds);

		// Update remaining context information.
		bWantsMouseCapture = ImGui::GetIO().WantCaptureMouse;
	}
}

void FImGuiContextProxy::BeginFrame(float DeltaTime)
{
	if (!bIsFrameStarted)
	{
		if( NetImGuiCanDrawProxy(this) )
		{
			SetAsCurrent();
			ImGuiIO& IO		= ImGui::GetIO();
			IO.DeltaTime	= DeltaTime;

			//@BeginKLMOD: use float instead of double
			IO.DisplaySize	= { static_cast<float>(DisplaySize.X), static_cast<float>(DisplaySize.Y) };
			ImGuiInterops::CopyInput(IO, InputState);
			ImGui::NewFrame();
		}

		InputState.ClearUpdateState();
		bIsFrameStarted = true;
		bIsDrawEarlyDebugCalled = false;
		bIsDrawDebugCalled = false;
	}
}

void FImGuiContextProxy::EndFrame()
{
	if (bIsFrameStarted)
	{
		if ( NetImGuiCanDrawProxy(this) )
		{
			// Prepare draw data (after this call we cannot draw to this context until we start a new frame).
			SetAsCurrent();
			ImGui::Render();

			// Update our draw data, so we can use them later during Slate rendering while ImGui is in the middle of the
			// next frame.
			UpdateDrawData(ImGui::GetDrawData());
		}
		bIsFrameStarted = false;
	}
}

// Is this context the current ImGui context.
bool FImGuiContextProxy::IsCurrentContext() const
{
	return ImGui::GetCurrentContext() == Context;
}

// Set this context as current ImGui context.
void FImGuiContextProxy::SetAsCurrent()
{
	ImGui::SetCurrentContext(Context);
}

void FImGuiContextProxy::UpdateDrawData(ImDrawData* DrawData)
{
	if (DrawData && DrawData->CmdListsCount > 0)
	{
		DrawLists.SetNum(DrawData->CmdListsCount, false);

		for (int Index = 0; Index < DrawData->CmdListsCount; Index++)
		{
			DrawLists[Index].TransferDrawData(*DrawData->CmdLists[Index]);
		}
	}
	else
	{
		// If we are not rendering then this might be a good moment to empty the array.
		DrawLists.Empty();
	}
}

void FImGuiContextProxy::BroadcastWorldEarlyDebug()
{
	if (ContextIndex != Utilities::INVALID_CONTEXT_INDEX)
	{
        FOnImGuiDelegate& WorldEarlyDebugEvent = FImGuiDelegatesContainer::Get().OnWorldEarlyDebug(ContextIndex);
        if (mWorld.IsValid() && WorldEarlyDebugEvent.IsBound())
		{
            WorldEarlyDebugEvent.Broadcast(*mWorld.Get());
		}
	}
}

void FImGuiContextProxy::BroadcastMultiContextEarlyDebug()
{
    FOnImGuiDelegate& MultiContextEarlyDebugEvent = FImGuiDelegatesContainer::Get().OnMultiContextEarlyDebug();
    if (mWorld.IsValid() && MultiContextEarlyDebugEvent.IsBound())
	{
		MultiContextEarlyDebugEvent.Broadcast(*mWorld.Get());
	}
}

void FImGuiContextProxy::BroadcastWorldDebug()
{
	if (DrawEvent.IsBound())
	{
        DrawEvent.Broadcast();
	}

	if (ContextIndex != Utilities::INVALID_CONTEXT_INDEX)
	{
        FOnImGuiDelegate& WorldDebugEvent = FImGuiDelegatesContainer::Get().OnWorldDebug(ContextIndex);
        if (mWorld.IsValid() && WorldDebugEvent.IsBound())
		{
            WorldDebugEvent.Broadcast(*mWorld.Get());
		}
	}
}

void FImGuiContextProxy::BroadcastMultiContextDebug()
{
    FOnImGuiDelegate& MultiContextDebugEvent = FImGuiDelegatesContainer::Get().OnMultiContextDebug();
    if (mWorld.IsValid() && MultiContextDebugEvent.IsBound())
	{
        MultiContextDebugEvent.Broadcast(*mWorld.Get());
	}
}
