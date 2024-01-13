// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/KLDebugImGuiServerObjectFeatures.h"
#include "Features/KLDebugImGuiServerUniqueFeatures.h"

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"
#include "Networking/Runtime/Public/Server/CachedConnection/KLDebugNetworkingPendingMessage.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FArchive;

class FKLDebugImGuiFeatureContainerBase;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate;
class FKLDebugImGuiNetworkingMessage_FeatureStatusData;
class FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate;
class FKLDebugImGuiNetworkingPendingMessage;
class FKLDebugNetworkingPendingMessage;
class FNetworkGUID;
class IKLDebugImGuiFeatureInterfaceBase;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerCacheConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerCacheConnection();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void GameThreadTick(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _ArchiveWriter);
    UE_NODISCARD bool RequiresGameThreadTick() const;

private:
    UE_NODISCARD FKLDebugImGuiServerObjectFeatures& GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD FKLDebugImGuiServerObjectFeatures* TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID);
    void RemoveObjectFeatures(const FNetworkGUID& _NetworkID);
    UE_NODISCARD TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> GetFeatureIndex(const FKLDebugImGuiFeatureContainerBase& _Container, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID) const;

    void GameThread_TickPendingMessages(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _ArchiveWriter);
    void GameThread_TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);

    UE_NODISCARD bool Rcv_HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FArchive& _Archive);
    UE_NODISCARD bool Rcv_HandleClientFeatureRequestUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FKLDebugImGuiNetworkingMessage_FeatureRequestUpdate& _Message, FArchive& _ArchiveWriter);
    UE_NODISCARD bool Recv_SelectableUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update);
    UE_NODISCARD bool Recv_UniqueUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update);

    UE_NODISCARD bool CanAddFeatureToTickList(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const;

    void GameThead_TickUniqueFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);
    void GameThread_TickObjectFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);

private:
    FKLDebugImGuiServerUniqueFeatures mUniqueFeatures;
    TArray<FKLDebugImGuiServerObjectFeatures> mFeaturesPerObject;
    TArray<FKLDebugNetworkingPendingMessage> mPendingMessages;
    TArray<uint8> mTempData;
    TArray<uint8> mTempCompressedData;
    TArray<uint8> mTempFeatureData;
};

inline FKLDebugImGuiServerObjectFeatures* FKLDebugImGuiServerCacheConnection::TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID)
{
    FKLDebugImGuiServerObjectFeatures* Feature = mFeaturesPerObject.FindByKey(_NetworkID);
    return Feature;
}

inline bool FKLDebugImGuiServerCacheConnection::RequiresGameThreadTick() const
{
    return !mUniqueFeatures.GetFeatures().IsEmpty() || !mFeaturesPerObject.IsEmpty() || !mPendingMessages.IsEmpty();
}

inline void FKLDebugImGuiServerCacheConnection::AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    mPendingMessages.Emplace(MoveTemp(_PendingMessage));
}
