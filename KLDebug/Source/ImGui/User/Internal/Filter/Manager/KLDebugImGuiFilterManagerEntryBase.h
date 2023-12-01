// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "StaticMemory/KLDebugImGuiStaticMemoryEntryBase.h"

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFilterManagerEntryBase : public TKLDebugImGuiStaticMemoryEntryBase<IKLDebugImGuiFilterInterface, FKLDebugImGuiFilterManagerEntryBase>
{
public:
    explicit FKLDebugImGuiFilterManagerEntryBase(const size_t _ClassSize);
};
