#pragma once

#include "Client/KLDebugImGuiNetworkingTCPClient.h"
#include "Subsystem/Engine/KLDebugImGuiClientData.h"

//modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingSubsystem_EngineBase.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionRunnableContainer.h"

//engine
#include "Containers/Array.h"
#include "Subsystems/EngineSubsystem.h"

#include "KLDebugImGuiClientSubsystem_Engine.generated.h"

class AController;
class AGameModeBase;
class APlayerController;
class FKLDebugImGuiFeatureStatusUpdateData;
class FString;

UCLASS(Transient)
class KLDEBUGIMGUICLIENT_API UKLDebugImGuiClientSubsystem_Engine final : public UKLDebugImGuiNetworkingSubsystem_EngineBase
{
    GENERATED_BODY()

public:
    //UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    //UEngineSubsystem

    static const UKLDebugImGuiClientSubsystem_Engine* TryGet();
    static UKLDebugImGuiClientSubsystem_Engine* TryGetMutable();

    void SetHasServerInitializedOwner(const bool _HasServerInitializedOwner);

private:
    //UKLDebugImGuiNetworkingSubsystem_EngineBase
    UE_NODISCARD FKLDebugImGuiNetworkingTCPBase* GetConnectionMutable() final;
    UE_NODISCARD const FKLDebugImGuiNetworkingTCPBase* GetConnection() const final;
    UE_NODISCARD bool IsValidWorld(UWorld& _World) const final;
    void OnWorldAdded(UWorld& _World) final;
    void OnWorldRemoved(UWorld& _World) final;

    ETickableTickType GetTickableTickTypeChild() const;
    TStatId GetStatId() const final;
    void Tick(float _DeltaTime) final;
    bool IsTickable() const final;
    //UKLDebugImGuiNetworkingSubsystem_EngineBase

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

private:
    TKLDebugImGuiNetworkingConnectionRunnableContainer<FKLDebugImGuiNetworkingTCPClient> mClientConnection;
    TArray<FKLDebugImGuiClientData> mClientsData;
    bool mHasServerInitializedOwner = false;
};

inline const UKLDebugImGuiClientSubsystem_Engine* UKLDebugImGuiClientSubsystem_Engine::TryGet()
{
    return TryGetMutable();
}

inline FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiClientSubsystem_Engine::GetConnectionMutable()
{
    return &mClientConnection.GetConnectionMutable();
}

inline const FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiClientSubsystem_Engine::GetConnection() const
{
    return &mClientConnection.GetConnection();
}

inline TStatId UKLDebugImGuiClientSubsystem_Engine::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiClientSubsystem_Engine, STATGROUP_Tickables);
}

inline bool UKLDebugImGuiClientSubsystem_Engine::IsTickable() const
{
    return Super::IsTickable() && mHasServerInitializedOwner;
}

inline ETickableTickType UKLDebugImGuiClientSubsystem_Engine::GetTickableTickTypeChild() const
{
    return ETickableTickType::Conditional;
}

inline void UKLDebugImGuiClientSubsystem_Engine::SetHasServerInitializedOwner(const bool _HasServerInitializedOwner)
{
    mHasServerInitializedOwner = _HasServerInitializedOwner;
}
