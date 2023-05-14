#pragma once

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

// engine
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerSelectableObject final : public FKLDebugImGuiFeatureVisualizerBase
{
public:
    explicit FKLDebugImGuiFeatureVisualizerSelectableObject(const FKLDebugImGuiFeatureContainerBase& _Container, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    UE_NODISCARD bool operator==(const UObject& _Object) const;

    // FKLDebugImGuiFeatureVisualizerBase
    UE_NODISCARD bool IsValid() const final;
    void              Render(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const final;
    // FKLDebugImGuiFeatureVisualizerBase

    UE_NODISCARD bool ShouldKeepAlive() const;

private:
    // FKLDebugImGuiFeatureVisualizerBase
    void DrawImGuiTree(const UWorld& _World) final;
    void DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) final;
    // FKLDebugImGuiFeatureVisualizerBase

private:
    TWeakObjectPtr<UObject> mObject;
    bool mKeepAlive = true;
};

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::operator==(const UObject& _Object) const
{
    return mObject.Get() == &_Object;
}

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::IsValid() const
{
    return mObject.IsValid();
}

inline bool FKLDebugImGuiFeatureVisualizerSelectableObject::ShouldKeepAlive() const
{
    return mKeepAlive;
}