#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSelectableObject.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSubsystem.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "InstancedStruct.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/UniquePtr.h"

#include "KLDebugImGuiWorldSubsystem.generated.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class IKLDebugImGuiFeatureInterfaceBase;

UCLASS(Transient)
class KLDEBUGIMGUI_API UKLDebugImGuiWorldSubsystem final : public UWorldSubsystem
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

private:
    TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem> GetVisualizer(const EContainerType _ContainerType, const FString& _VisualizerName, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) const;

    void DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const;
    void DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);
    void DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

private:
    TArray<TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem>, TInlineAllocator<static_cast<int32>(EContainerType::COUNT)>> mSubsystemsFeaturesVisualizer;
    TArray<FKLDebugImGuiFeatureVisualizerSelectableObject> mSelectedObjectsVisualizers;
    FString                                                mImGuiTreeName;
    FInstancedStruct                                       mImGuiWindow;
};
