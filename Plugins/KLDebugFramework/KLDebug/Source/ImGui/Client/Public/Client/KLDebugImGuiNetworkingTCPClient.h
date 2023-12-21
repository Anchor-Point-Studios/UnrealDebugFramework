// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientWorldCacheConnection.h"
#include "ServerPortGetter/KLDebugImGuiClientServerPortGetterBase.h"

// modules
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionDelegates.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"

class FArchive;
class FKLDebugImGuiClientGameThreadContext;
class UKLDebugNetworkingArbitrerSettings;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiNetworkingTCPClient final : public FKLDebugImGuiNetworkingTCPBase
{
public:
    // FKLDebugImGuiNetworkingTCPBase
    bool Init() final;
    void RunChild() final;
    void Exit() final;
    // FKLDebugImGuiNetworkingTCPBase

    void TickGameThread(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    void RemoveCachedConnection(const int32 _Index);
    void TickPortGetter();
    void TickConnections();

    void GameThread_InitServerPortGetter(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_RemoveInvalidWorlds(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_NewWorlds(const FKLDebugImGuiClientGameThreadContext& _Context);
    void GameThread_TickImGuiData(FKLDebugImGuiClientGameThreadContext& _Context);

private:
    TArray<FKLDebugImGuiClientWorldCacheConnection> mCachedConnections;
    TUniquePtr<IKLDebugImGuiClientServerPortGetterBase> mServerPortGetter;
};
