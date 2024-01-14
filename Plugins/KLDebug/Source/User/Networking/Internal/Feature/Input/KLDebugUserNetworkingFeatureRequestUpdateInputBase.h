// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTemplate.h"

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureRequestUpdateInputBase : public FNoncopyable
{
public:
    explicit FKLDebugUserNetworkingFeatureRequestUpdateInputBase(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FArchive& GetWriter() const;
    UE_NODISCARD FArchive& GetReader() const;

private:
    const UWorld& mWorld;
    FArchive& mReaderArchive;
    FArchive& mWriterArchive;
};

inline const UWorld& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetWriter() const
{
    return mWriterArchive;
}

inline FArchive& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetReader() const
{
    return mReaderArchive;
}