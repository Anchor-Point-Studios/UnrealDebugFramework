#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "Templates/UnrealTemplate.h"
#include "UObject/Object.h"

class KLDEBUGIMGUI_API FKLDebugImGuiGatherFeatureInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiGatherFeatureInput(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const EContainerType _ContainerType, const UObject* _Object, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

    void SetFeatureData(IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, FKLDebugImGuiFeatureContext_Base* _FeatureContext);

    UE_NODISCARD const FKLDebugImGuiFeaturesTypesContainerManager& GetContainerManager() const;
    UE_NODISCARD const UObject* TryGetObject() const;
    UE_NODISCARD EContainerType GetContainerType() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase* TryGetFeatureInterface() const;
    UE_NODISCARD FKLDebugImGuiFeatureContext_Base* TryGetFeatureContext() const;

private:
    const FKLDebugImGuiFeaturesTypesContainerManager& mFeaturesContainerManager;
    const UObject* mObject = nullptr;
    EContainerType mContainerType = EContainerType::COUNT;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;

    IKLDebugImGuiFeatureInterfaceBase* mInterfaceBase = nullptr;
    FKLDebugImGuiFeatureContext_Base* mFeatureContext = nullptr;
};

inline void FKLDebugImGuiGatherFeatureInput::SetFeatureData(IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, FKLDebugImGuiFeatureContext_Base* _FeatureContext)
{
    mInterfaceBase = &_FeatureInterface;
    mFeatureContext = _FeatureContext;
}

inline const FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiGatherFeatureInput::GetContainerManager() const
{
    return mFeaturesContainerManager;
}

inline const UObject* FKLDebugImGuiGatherFeatureInput::TryGetObject() const
{
    return mObject;
}

inline EContainerType FKLDebugImGuiGatherFeatureInput::GetContainerType() const
{
    return mContainerType;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiGatherFeatureInput::GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline IKLDebugImGuiFeatureInterfaceBase* FKLDebugImGuiGatherFeatureInput::TryGetFeatureInterface() const
{
    return mInterfaceBase;
}

inline FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiGatherFeatureInput::TryGetFeatureContext() const
{
    return mFeatureContext;
}

