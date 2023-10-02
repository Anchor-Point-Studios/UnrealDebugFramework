#pragma once

//modules
#include "ImGui/Networking/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"
#include "Networking/Runtime/Public/Common/KLDebugNetworkingManager_Base.h"

//engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Delegates/IDelegateInstance.h"
#include "Misc/NetworkGuid.h"
#include "Templates/SharedPointer.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FInternetAddr;
class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTypesContainerManager;
class FNetBitWriter;
class FSocket;
class FString;
class UKLDebugImGuiWorldSubsystem;
class UPackageMap;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientManager final : public FKLDebugNetworkingManager_Base
{
public:
    //FKLDebugImGuiNetworkManager_Base
    UE_NODISCARD bool IsSocketRegistered() const;
    //FKLDebugImGuiNetworkManager_Base

private:
    //FKLDebugImGuiNetworkManager_Base
    void Tick(const float _DeltaTime) final;
    void InitFromWorldChild(UWorld& _World) final;
    void ClearFromWorldChild(const UWorld& _World) final;
    //FKLDebugImGuiNetworkManager_Base

    void InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize);

    void InitWorldDelegates();
    void ClearWorldDelegates();

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

    void TickReadData();
    void TickWriteData();

    void TryReconnect();

    void WritePendingFeaturesStatusUpdate(FNetBitWriter& _BitWriter);

    void ReadData(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UKLDebugImGuiWorldSubsystem& _ImGuiWorldSubsystem, FBitReader& _Reader);

    UE_NODISCARD UPackageMap* GetServerPackageMap() const;

private:
    TArray<uint8> mReceiverDataBuffer;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;
    TSharedPtr<FInternetAddr> mServerAddress;
    FDelegateHandle mOnFeaturesUpdatedDelegateHandle;
    FSocket* mServerSocket = nullptr;
    int32 mSendBufferSize = 0;
    float mReconnectionTime = 1.f;
    float mLastTimeTryToConnect = 0.f;
    bool mHasInitializedAfterConnection = false;
};

inline bool FKLDebugImGuiClientManager::IsSocketRegistered() const
{
    return mServerSocket != nullptr;
}