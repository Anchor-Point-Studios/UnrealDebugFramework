// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugFeatureTickInput_Base.h"

class UObject;

class KLDEBUGIMGUIUSER_API FKLDebugFeatureTickInput_Selectable final : public FKLDebugFeatureTickInput_Base
{
public:
    explicit FKLDebugFeatureTickInput_Selectable(const UWorld& _World, const UObject& _Owner, FKLDebugImGuiFeatureContext_Base* _Context);

    UE_NODISCARD const UObject& GetOwner() const;

private:
    const UObject& mOwner;
};

inline const UObject& FKLDebugFeatureTickInput_Selectable::GetOwner() const
{
    return mOwner;
}
