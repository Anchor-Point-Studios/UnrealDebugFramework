#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterfaceBase::DrawImGui(const UWorld& _World)
{
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin(TCHAR_TO_ANSI(*GetWindowName()), nullptr, WindowFlags))
    {
        return;
    }

    DrawImGuiChild(_World);
    ImGui::End();
}
