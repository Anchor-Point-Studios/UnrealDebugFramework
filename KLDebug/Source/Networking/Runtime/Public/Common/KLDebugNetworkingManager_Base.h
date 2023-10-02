#pragma once

//utils module
#include "Utils/Public/Tick/KLDebugUtilsTickObject.h"

// engine
#include "Containers/Array.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FSocket;
class FString;
class UKLDebugNetworkingSettings;
class UWorld;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingManager_Base : public FNoncopyable
{
public:
    virtual ~FKLDebugNetworkingManager_Base() = default;

    void InitFromWorld(UWorld& _World);
    UE_NODISCARD virtual bool IsSocketRegistered() const = 0;

    void ClearFromWorld(const UWorld& _World);

protected:
    virtual void InitFromWorldChild(UWorld& _World);
    virtual void ClearFromWorldChild(const UWorld& _World);
    virtual void Tick(const float _DeltaTime) = 0;

    void InitTick(UWorld& _World);
    UE_NODISCARD const UKLDebugNetworkingSettings& GetNetworkConfig() const;

    void UnregisterTick();
    UE_NODISCARD bool SendData(FSocket& _Socket, FBitWriter& _Writer) const;

    UE_NODISCARD UWorld& GetWorldMutable() const;
    UE_NODISCARD const UWorld& GetWorld() const;

private:
    FKLDebugUtilsTickObject mTickObject;
    TWeakObjectPtr<UWorld> mWorld;
};

inline void FKLDebugNetworkingManager_Base::InitFromWorldChild(UWorld& _World)
{
}

inline void FKLDebugNetworkingManager_Base::ClearFromWorldChild(const UWorld& _World)
{
}

inline const UWorld& FKLDebugNetworkingManager_Base::GetWorld() const
{
    return GetWorldMutable();
}