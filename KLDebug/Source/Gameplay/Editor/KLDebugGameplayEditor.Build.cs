using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class KLDebugGameplayEditor : ModuleRules
{
    public KLDebugGameplayEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[]
            {
                Path.Combine(ModuleDirectory, "Private"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "KLDebugGameplayRuntime",
                "KLDebugUtils",

                //engine
                "Engine",
                "EnhancedInput",
                "InputCore",
                "InputEditor"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "UnrealEd"
            }
         );
    }
}
