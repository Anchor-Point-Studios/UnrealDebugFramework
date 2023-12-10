// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Selectable.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "InstancedStruct.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/UniquePtr.h"

#include "KLDebugImGuiWorldSubsystem.generated.h"

class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTypesContainerManager;
class IKLDebugImGuiFeatureInterfaceBase;
class FKLDebugImGuiGatherFeatureInput;

UCLASS(Transient)
class KLDEBUGIMGUIFRAMEWORK_API UKLDebugImGuiWorldSubsystem final : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // UWorldSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    void OnWorldBeginPlay(UWorld& _World) final;
    // UWorldSubsystem

    void DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void Render(const UWorld& _CurrentWorldUpdated, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const;

    UE_NODISCARD static UKLDebugImGuiWorldSubsystem* TryGetMutable(const UObject& _Object);

    // this can be called externally when an object is selected. For example from the editor module of the imgui
    // to support editor selection
    void OnObjectSelected(UObject& _Object);

    UE_NODISCARD FDelegateHandle BindOnImGuiFeatureStateUpdated(const FOnImGuiFeatureStateUpdated::FDelegate& _Delegate);
    void UnbindOnImGuiFeatureStateUpdated(FDelegateHandle& _Handle);

    void TryGatherFeatureAndContext(FKLDebugImGuiGatherFeatureInput& _Input) const;

private:
    void DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

private:
    FKLDebugImGuiFeatureVisualizer_Unique mUniqueFeaturesVisualizer;
    TArray<FKLDebugImGuiFeatureVisualizer_Selectable> mSelectedObjectsVisualizers;
    FString mImGuiTreeName;
    FInstancedStruct mImGuiWindow;
    FOnImGuiFeatureStateUpdated mOnFeaturesUpdatedDelegate;
    bool mShouldStoreDelta = false;
};

inline FDelegateHandle UKLDebugImGuiWorldSubsystem::BindOnImGuiFeatureStateUpdated(const FOnImGuiFeatureStateUpdated::FDelegate& _Delegate)
{
    return mOnFeaturesUpdatedDelegate.Add(_Delegate);
}

inline void UKLDebugImGuiWorldSubsystem::UnbindOnImGuiFeatureStateUpdated(FDelegateHandle& _Handle)
{
    mOnFeaturesUpdatedDelegate.Remove(_Handle);
    _Handle.Reset();
}