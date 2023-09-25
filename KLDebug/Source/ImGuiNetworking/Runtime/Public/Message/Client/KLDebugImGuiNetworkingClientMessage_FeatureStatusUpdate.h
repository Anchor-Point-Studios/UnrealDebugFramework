#pragma once

#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//imgui module
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

//engine
#include "Misc/NetworkGuid.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate final : public FKLDebugImGuiNetworkingMessageBase
{
private:
    using FeatureData = TPair<KL::Debug::ImGui::Features::Types::FeatureIndex, bool>;

public:
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate() = default;
    explicit FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID);

    //FKLDebugImGuiNetworkingMessageBase
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMessageType() const final;
    UE_NODISCARD bool CanWrite(const UWorld& _World) const final;
    void WriteChild(const UWorld& _World, FBitWriter& _BitWriter) final;
    void ReadChild(const UWorld& _World, FBitReader& _BitReader) final;
    //FKLDebugImGuiNetworkingMessageBase

    UE_NODISCARD bool IsEqual(const EContainerType _ContainerType, const FNetworkGUID& _NetworkID) const;
    void AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status);
    UE_NODISCARD bool IsFullyRemoved() const;
    void SetFullyRemoved();
    void ClearFullyRemoved();

private:
    TArray<FeatureData> mFeatureData;
    FNetworkGUID mNetworkID;
    bool mFullyRemoved = false;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::GetMessageType() const
{
    return EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const bool _Status)
{
    mFeatureData.Emplace(_FeatureIndex, _Status);
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::CanWrite(const UWorld& _World) const
{
    return mNetworkID.IsValid();
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::IsEqual(const EContainerType _ContainerType, const FNetworkGUID& _NetworkID) const
{
    return mContainerType == _ContainerType && mNetworkID == _NetworkID;
}

inline bool FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::IsFullyRemoved() const
{
    return mFullyRemoved;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::SetFullyRemoved()
{
    mFullyRemoved = true;
}

inline void FKLDebugImGuiNetworkingClientMessage_FeatureStatusUpdate::ClearFullyRemoved()
{
    mFullyRemoved = false;
}