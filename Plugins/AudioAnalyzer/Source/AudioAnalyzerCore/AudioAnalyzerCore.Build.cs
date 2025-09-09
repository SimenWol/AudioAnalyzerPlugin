using UnrealBuildTool;

public class AudioAnalyzerCore : ModuleRules
{
    public AudioAnalyzerCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}