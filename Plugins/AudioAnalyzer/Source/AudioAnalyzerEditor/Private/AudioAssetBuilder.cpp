#include "AudioAssetBuilder.h"

#include "Sound/SoundWave.h"
#include "AssetToolsModule.h"
#include "Factories/Factory.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Log.h"

// editor only
#if WITH_EDITOR

// #include "AudioSynesthesiaNRT.h"
#include "AudioSynesthesiaNRTFactory.h"
#include "LoudnessNRT.h"
// #include "LoudnessNRTFactory.h"
// #include "OnsetNRTFactory.h"
// #include "ConstantQNRTFactory.h"

FGeneratedNRTAssets UAudioAssetBuilder::BuildAllAssets(USoundWave* SourceAudio, const FString& PackagePath)
{
    FGeneratedNRTAssets analysers;

    if (!SourceAudio)
    {
        UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("No SourceAudio, building assets cancelled."));
        return analysers;
    }

    FString BaseName = SourceAudio->GetName();

    // TODO: add bools to individually enable packaging certain analyzers.
    analysers.LoudnessNRT = BuildLoudnessAsset(SourceAudio, PackagePath, BaseName + TEXT("_LoudnessNRT"));
    // BuildOnsetAsset(SourceAudio, PackagePath, BaseName + TEXT("_OnsetNRT"));
    // BuildConstantQAsset(SourceAudio, PackagePath, BaseName + TEXT("_ConstantQNRT"));

    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("Successfully generated all NRT analysers."));
    return analysers;
}

ULoudnessNRT* UAudioAssetBuilder::BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName)
{
    if (!SourceAudio) return nullptr;

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

    // Create factory
    UAudioSynesthesiaNRTFactory* Factory = NewObject<UAudioSynesthesiaNRTFactory>();
    Factory->AudioSynesthesiaNRTClass = ULoudnessNRT::StaticClass();

    // Create asset
    UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, ULoudnessNRT::StaticClass(), Factory);
    
    ULoudnessNRT* LoudnessAsset = Cast<ULoudnessNRT>(NewAsset);
    if (LoudnessAsset)
    {    
        ULoudnessNRTSettings* Settings = NewObject<ULoudnessNRTSettings>();
        
        Settings->AnalysisPeriod = 0.01f; // 100 samples per second
        Settings->CurveType = ELoudnessNRTCurveTypeEnum::A;
        
        LoudnessAsset->Sound = SourceAudio;
        LoudnessAsset->Settings = Settings;

        FAssetRegistryModule::AssetCreated(LoudnessAsset);
        LoudnessAsset->MarkPackageDirty();
    }

    return LoudnessAsset;
}

#endif