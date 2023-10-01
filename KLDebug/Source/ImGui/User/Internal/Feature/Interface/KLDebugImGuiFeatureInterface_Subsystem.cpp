#include "Feature/Interface/KLDebugImGuiFeatureInterface_Subsystem.h"

#include "Helpers/KLDebugImGuiHelpers.h"

//modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_Subsystem::DrawImGui(const FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem& _Context)
{
    auto Callback = [this, &_Context]()->void {
        DrawImGuiChild(_Context);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Context.GetIsWindowOpen(),
        Callback);
}