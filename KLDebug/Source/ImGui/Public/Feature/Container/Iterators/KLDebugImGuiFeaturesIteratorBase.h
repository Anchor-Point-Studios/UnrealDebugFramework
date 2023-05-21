#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/ChooseClass.h"
#include "Templates/EnableIf.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureData;
class IKLDebugImGuiFeatureInterfaceBase;

template<class Child, bool IsConst>
class FKLDebugImGuiFeaturesIteratorBase : public FNoncopyable
{
protected:
    typedef typename TChooseClass<IsConst, const TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>, TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>>::Result PoolType;

public:
    explicit FKLDebugImGuiFeaturesIteratorBase(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, PoolType& _FeaturesPool);
    void operator++();

    template<typename FeatureInterfaceType>
    UE_NODISCARD const FeatureInterfaceType& GetFeatureInterfaceCasted() const;

    template<typename FeatureInterfaceType, bool Enabled = IsConst, typename TEnableIf<!Enabled, bool>::Type = true>
    UE_NODISCARD FeatureInterfaceType& GetFeatureInterfaceCastedMutable() const;

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndex() const;
    UE_NODISCARD int32                                           GetFeaturesCount() const;
    UE_NODISCARD const FKLDebugImGuiFeatureData&                 GetFeatureData() const;

protected:
    template<bool Enabled = IsConst, typename TEnableIf<!Enabled, bool>::Type = true>
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase&       GetFeatureMutable() const;
    UE_NODISCARD const IKLDebugImGuiFeatureInterfaceBase& GetFeature() const;

protected:
    // the data contains the offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    const TArray<FKLDebugImGuiFeatureData>& mFeatureData;

    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    PoolType& mFeaturesPool;

    uint32 mIndex = 0;
};

template<class Child, bool IsConst>
FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::FKLDebugImGuiFeaturesIteratorBase(const TArray<FKLDebugImGuiFeatureData>& _FeatureData, PoolType& _FeaturesPool)
    : mFeatureData(_FeatureData)
    , mFeaturesPool(_FeaturesPool)
{
}

template<class Child, bool IsConst>
inline void FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::operator++()
{
    ++mIndex;
}

template<class Child, bool IsConst>
inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeatureDataIndex() const
{
    return mIndex;
}

template<class Child, bool IsConst>
template<typename FeatureInterfaceType, bool Enabled, typename TEnableIf<!Enabled, bool>::Type>
inline FeatureInterfaceType& FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeatureInterfaceCastedMutable() const
{
    checkf(GetFeature().IsDerivedFrom<FeatureInterfaceType>(), TEXT("casting to wrong type"));
    return reinterpret_cast<FeatureInterfaceType&>(GetFeatureMutable());
}

template<class Child, bool IsConst>
template<typename FeatureInterfaceType>
inline const FeatureInterfaceType& FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeatureInterfaceCasted() const
{
    checkf(GetFeature().IsDerivedFrom<FeatureInterfaceType>(), TEXT("casting to wrong type"));
    return reinterpret_cast<const FeatureInterfaceType&>(GetFeature());
}

template<class Child, bool IsConst>
template<bool Enabled, typename TEnableIf<!Enabled, bool>::Type>
inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeatureMutable() const
{
    return static_cast<const Child*>(this)->GetFeatureMutableChild();
}

template<class Child, bool IsConst>
inline const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeature() const
{
    return static_cast<const Child*>(this)->GetFeatureChild();
}

template<class Child, bool IsConst>
inline int32 FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeaturesCount() const
{
    return mFeatureData.Num();
}

template<class Child, bool IsConst>
inline const FKLDebugImGuiFeatureData& FKLDebugImGuiFeaturesIteratorBase<Child, IsConst>::GetFeatureData() const
{
    return mFeatureData[mIndex];
}