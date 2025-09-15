using UnrealBuildTool;

public class AudioAnalyzerEditor : ModuleRules
{
    public AudioAnalyzerEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AudioSynesthesia", "AudioSynesthesiaEditor", "AudioAnalyzerCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "AssetTools", "AssetRegistry", "Slate", "SlateCore", "PropertyEditor" });
    }
}