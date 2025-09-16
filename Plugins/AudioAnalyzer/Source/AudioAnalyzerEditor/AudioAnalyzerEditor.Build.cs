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
                "UnrealEd",
                "AudioSynesthesia",
                "Slate",
                "SlateCore",
                "PropertyEditor",
                "AudioAnalyzerCore"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AssetTools",
                "AssetRegistry",
                "AudioSynesthesiaEditor"
            }
        );
    }
}