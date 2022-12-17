#include "Picker/KLDebugUtilsPicker.h"

#include "Picker/KLDebugUtilsPickerScoredObjects.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Box.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/Object.h"

namespace KL::Debug::Utils::Picker
{
    template<typename Callback>
    void GatherAllObjects(const UWorld& _World, const TSubclassOf<AActor>& _ActorType, TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects, const Callback& _Callback)
    {
        float Distance = 0.f;
        for (TActorIterator<AActor> It(&_World, _ActorType); It; ++It)
        {
            AActor* Actor = *It;
            if (_Callback(*Actor, Distance))
            {
                continue;
            }

            _OutObjects.Emplace(Distance, *Actor);
        }
    }
}

///////////////////////////////////////////////////////////////////////////

FKLDebugUtilsPicker::FKLDebugUtilsPicker()
    : mActorType(AActor::StaticClass())
{
}

UObject* FKLDebugUtilsPicker::GetPickedObject(const UWorld& _World) const
{
    TArray<FKLDebugUtilsPickerScoredObjects> Objects;
    GatherAllObjects(_World, Objects);
    if (Objects.IsEmpty())
    {
        return nullptr;
    }

    SortByDistance(Objects);
    ApplyScores(Objects);
    return Objects[0].GetScore() == 0.f ? nullptr : Objects[0].GetObject();
}

void FKLDebugUtilsPicker::SetActorType(UClass& _Class)
{
    mActorType = &_Class;
}

void FKLDebugUtilsPicker::SetInterfaceType(UClass& _Class)
{
    mActorInterface = &_Class;
}

bool FKLDebugUtilsPicker::GetCenterLocationFromScreenCoord(const UWorld& _World, FVector& _OutLocation, FVector& _OutDirecton) const
{
    const APlayerController* PlayerController = _World.GetFirstPlayerController();
    if (!PlayerController)
    {
        return false;
    }

    const APlayerCameraManager* PlayerCamera = PlayerController->PlayerCameraManager;
    if (!PlayerCamera)
    {
        return false;
    }

    FRotator CamRot;
    PlayerCamera->GetCameraViewPoint(_OutLocation, CamRot);
    _OutDirecton = UKismetMathLibrary::GetForwardVector(CamRot);
    return true;
}

void FKLDebugUtilsPicker::GatherAllObjects(const UWorld& _World, TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    FVector Location;
    FVector Directon;
    if (!GetCenterLocationFromScreenCoord(_World, Location, Directon))
    {
        return;
    }

    _OutObjects.Reserve(1000);

    if (mActorTag.IsNone() && mActorInterface.Get() == nullptr)
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool {
            return IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else if (!mActorTag.IsNone() && mActorInterface.Get() != nullptr)
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool
        {
            return _Actor.ActorHasTag(mActorTag) && _Actor.GetClass()->ImplementsInterface(mActorInterface.Get())
                && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else if (!mActorTag.IsNone())
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool
        {
            return _Actor.ActorHasTag(mActorTag) && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }
    else
    {
        auto Lambda = [this, &Location, &Directon](const AActor& _Actor, float& _OutDistance) -> bool
        {
            return _Actor.GetClass()->ImplementsInterface(mActorInterface.Get()) && IsRightDistance(_Actor, Location, Directon, _OutDistance);
        };

        KL::Debug::Utils::Picker::GatherAllObjects(_World, mActorType, _OutObjects, Lambda);
    }

    _OutObjects.Shrink();
}

bool FKLDebugUtilsPicker::IsRightDistance(const AActor& _Actor, const FVector& _Position, const FVector& _Direction, float& _OutDistance) const
{
    FVector Pos;
    FVector Extents;
    _Actor.GetActorBounds(true, Pos, Extents);
    //const FBox Box{Pos - Extents, Pos + Extents};

    //FMath::LineBoxIntersection(InBox, RootPos, TipPos, TipPos - RootPos)
    //Box.ComputeSquaredDistanceToPoint()
    
    //NOTE THIS IS WRONG: need to transform the obejcts in screen space.
    //also need to take into account the bounding box of the actor (is any)
    /*
    * APlayerController * PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    FVector2D FinalLocation;
    FVector2D viewportDimensions;
    FVector2D ResultVector;

    float ResultLength;
    int viewportX;
    int viewportY;


    PlayerController-&gt;GetViewportSize(viewportX,viewportY);
    viewportDimensions = FVector2D(viewportX, viewportY);
    PlayerController-&gt;ProjectWorldLocationToScreen(Pos, FinalLocation);

    ResultVector = (viewportDimensions/2) - FinalLocation;
    ResultLength = ResultVector.Size();
    */
            _OutDistance = FMath::PointDistToLine(Pos, _Direction, _Position);
    return _OutDistance < mMaxDistance;
}

void FKLDebugUtilsPicker::SortByDistance(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    _OutObjects.Sort([](const FKLDebugUtilsPickerScoredObjects& _Left, const FKLDebugUtilsPickerScoredObjects& _Right) -> bool {
        return _Left.GetDistance() < _Right.GetDistance();
    });
}

void FKLDebugUtilsPicker::ApplyScores(TArray<FKLDebugUtilsPickerScoredObjects>& _OutObjects) const
{
    _OutObjects.Sort([](const FKLDebugUtilsPickerScoredObjects& _Left, const FKLDebugUtilsPickerScoredObjects& _Right) -> bool {
        return _Left.GetScore() < _Right.GetScore();
    });
}
