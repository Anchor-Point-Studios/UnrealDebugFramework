#pragma once

#include "Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdateData.h"
#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

//engine
#include "Misc/NetworkGuid.h"

class FName;
class UWorld;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate final : public FKLDebugImGuiNetworkingMessageBase
{
public:
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate() = default;
    explicit FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate(const FNetworkGUID& _NetworkID, const EImGuiInterfaceType _ContainerType);

    //FKLDebugImGuiNetworkingMessageBase
    UE_NODISCARD EKLDebugNetworkMessageTypes GetMessageType() const final;
    UE_NODISCARD bool CanWrite(const UWorld& _World) const final;
    void WriteChild(const UWorld& _World, FArchive& _BitWriter) final;
    void ReadChild(const UWorld& _World, FArchive& _BitReader) final;
    //FKLDebugImGuiNetworkingMessageBase

    //client
    UE_NODISCARD bool Client_IsEqual(const EImGuiInterfaceType _ContainerType, const FNetworkGUID& _NetworkID) const;
    void Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID, const bool _Status);
    void Client_SetFullyRemoved();
    void Client_ClearFullyRemoved();
    //client

    //server
    UE_NODISCARD bool Server_IsFullyRemoved() const;
    UE_NODISCARD const FNetworkGUID& Server_GetNetworkID() const;
    UE_NODISCARD EImGuiInterfaceType Server_GetContainerType() const;
    UE_NODISCARD const TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData>& Server_GetFeaturesData() const;
    //server

private:
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData> mFeatureData;
    FNetworkGUID mNetworkID;
    bool mFullyRemoved = false;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
};

inline EKLDebugNetworkMessageTypes FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::GetMessageType() const
{
    return EKLDebugNetworkMessageTypes::Client_FeatureStatusUpdate;
}

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_AddFeatureUpdate(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID, const bool _Status)
{
    mFeatureData.Emplace(_FeatureNameID, _FeatureIndex, _Status);
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::CanWrite(const UWorld& _World) const
{
    return mNetworkID.IsValid() && mContainerType != EImGuiInterfaceType::COUNT;
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_IsEqual(const EImGuiInterfaceType _ContainerType, const FNetworkGUID& _NetworkID) const
{
    return mContainerType == _ContainerType && mNetworkID == _NetworkID;
}

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_SetFullyRemoved()
{
    mFullyRemoved = true;
}

inline void FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Client_ClearFullyRemoved()
{
    mFullyRemoved = false;
}

inline bool FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_IsFullyRemoved() const
{
    return mFullyRemoved;
}

inline const FNetworkGUID& FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetNetworkID() const
{
    return mNetworkID;
}

inline EImGuiInterfaceType FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetContainerType() const
{
    return mContainerType;
}

inline const TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdateData>& FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate::Server_GetFeaturesData() const
{
    return mFeatureData;
}

