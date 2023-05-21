#pragma once

// imgui module
#include "ImGui/Public/Filter/Interface/KLDebugImGuiFilterInterface.h"

class UObject;

class KLDEBUGEXAMPLES_API KLDebugImGuiFilterAI final : public IKLDebugImGuiFilterInterface
{
    KL_DEBUG_FILTER_GENERATE(AI)

public:
    UE_NODISCARD bool IsObjectValid(const UObject& _Object) const final;
};