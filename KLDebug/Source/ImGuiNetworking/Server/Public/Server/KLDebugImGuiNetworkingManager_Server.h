#pragma once

#include "Server/KLDebugImGuiNetworkingCacheConnection.h"

//networking runtime module
#include "ImGuiNetworking/Runtime/Public/Common/KLDebugImGuiNetworkingManager_Base.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Templates/RefCounting.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FSocket;
class FString;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworkingManager_Server final : public FKLDebugImGuiNetworkingManager_Base
{
public:
    //FKLDebugImGuiNetworkManager_Base
    UE_NODISCARD bool IsSocketRegistered() const;
    //FKLDebugImGuiNetworkManager_Base

private:
    //FKLDebugImGuiNetworkManager_Base
    void Tick(const float _DeltaTime) final;
    void InitChild(UWorld& _World) final;
    void ClearChild() final;
    //FKLDebugImGuiNetworkManager_Base

    void InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize);

    void TickListenerSocket();
    void TickConnections();

    void ReceiveConnectionData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FSocket& _ClientSocket);
    void SendConnectionData(FSocket& _ClientSocket) const;

    void ReadData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader);

    UE_NODISCARD UPackageMap* GetClientPackageMap(const UWorld& _World, const FSocket& _ClientSocket) const;

    void HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader);

private:
    TArray<TRefCountPtr<FKLDebugImGuiNetworkingCacheConnection>> mConnectedSockets;
    TArray<uint8> mReceiverDataBuffer;
    TMap<FName, uint16> mFeatureToContainerIndex;
    FSocket* mListenerSocket = nullptr;
    uint32 mClientReadBufferSize = 0;
    uint32 mClientWriteBufferSize = 0;
};

inline bool FKLDebugImGuiNetworkingManager_Server::IsSocketRegistered() const
{
    return mListenerSocket != nullptr;
}