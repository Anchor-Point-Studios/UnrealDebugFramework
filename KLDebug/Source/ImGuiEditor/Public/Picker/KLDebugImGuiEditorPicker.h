#pragma once

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Templates/UnrealTemplate.h"

class UObject;

class KLDEBUGIMGUIEDITOR_API FKLDebugImGuiEditorPicker final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();

private:
    void Clear();

    void OnImGuiEnable(const bool _IsEnable);
    void OnSelection(UObject* _Object) const;

private:
    FDelegateHandle mOnImGuiStateChangeHandle;
    FDelegateHandle mOnSelectionHandle;
};