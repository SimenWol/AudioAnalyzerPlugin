using UnrealBuildTool;

public class AudioAnalyzerEditor : ModuleRules
{
    public AudioAnalyzerEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "AudioSynesthesia",
                "PropertyEditor",
                "AudioAnalyzerCore"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "UnrealEd"
            }
        );
    }
}