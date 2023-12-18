// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Luncher/KLDebugNetworkingArbitrerLuncher.h"

#if PLATFORM_WINDOWS
#include "Luncher/Win/KLDebugNetworkingArbitrerLuncher_Win.h"
#elif PLATFORM_UNIX
#include "Luncher/Linux/KLDebugNetworkingArbitrerLuncher_Linux.h"
#endif

// engine
#include "Containers/UnrealString.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Templates/SharedPointer.h"

namespace KL::Debug::Networking::Arbitrer
{
    /////////////////////////////////////////////////////
    /// private
    static bool HasStartArbitrer = false;

    /////////////////////////////////////////////////////
    /// public

    void TryLunchArbitrer()
    {
        static const FString ExecutablePath = FString::Format(TEXT("{0}/Resources/Arbitrer/KLDebugArbitrer.exe"), { *(IPluginManager::Get().FindPlugin("KLDebug")->GetBaseDir()) });
        if (HasStartArbitrer)
        {
            return;
        }

        const FString CmdLine = FCommandLine::Get();
        const bool CreateConsole = FParse::Param(*CmdLine, TEXT("showarbitrerlog"));

        KL::Debug::Networking::Arbitrer::LunchArbitrer(ExecutablePath, CreateConsole);
        HasStartArbitrer = true;
    }

    void TryCloseArbitrer()
    {
        KL::Debug::Networking::Arbitrer::CloseArbitrer();
        HasStartArbitrer = false;
    }

}    // namespace KL::Debug::Networking::Arbitrer
