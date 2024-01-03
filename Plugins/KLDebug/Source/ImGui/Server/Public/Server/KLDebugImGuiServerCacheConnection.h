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
class FKLDebugImGuiNetworkingMessage_FeatureStatusData;
class FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate;
class FKLDebugImGuiNetworkingPendingMessage;
class FKLDebugNetworkingPendingMessage;
class FNetworkGUID;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerCacheConnection final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerCacheConnection();

    void AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage);

    void ReadData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer);
    void WriteData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);

    UE_NODISCARD bool NeedsTicking() const;
    UE_NODISCARD bool HasPendingData() const;

private:
    UE_NODISCARD FKLDebugImGuiServerObjectFeatures& GetOrAddFeaturesPerObject(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD FKLDebugImGuiServerObjectFeatures* TryGetFeaturesPerObjectMutable(const FNetworkGUID& _NetworkID);
    void RemoveObjectFeatures(const FNetworkGUID& _NetworkID);
    UE_NODISCARD TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> GetFeatureIndex(const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusData& _Data) const;

    UE_NODISCARD bool Rcv_HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FArchive& _Archive);
    UE_NODISCARD bool Recv_SelectableUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update);
    UE_NODISCARD bool Recv_UniqueUpdate(const UWorld& _World, const FKLDebugImGuiFeatureContainerBase& _Container, const FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate& _Update);

    void Write_ConnectionFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);
    void Write_UniqueFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);
    void Write_ObjectFeatures(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainer, FArchive& _Archive);

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

inline bool FKLDebugImGuiServerCacheConnection::NeedsTicking() const
{
    return !mUniqueFeatures.GetFeatures().IsEmpty() || !mFeaturesPerObject.IsEmpty();
}

inline void FKLDebugImGuiServerCacheConnection::AddPendingMessage(FKLDebugNetworkingPendingMessage&& _PendingMessage)
{
    mPendingMessages.Emplace(MoveTemp(_PendingMessage));
}

inline bool FKLDebugImGuiServerCacheConnection::HasPendingData() const
{
    return !mPendingMessages.IsEmpty();
}