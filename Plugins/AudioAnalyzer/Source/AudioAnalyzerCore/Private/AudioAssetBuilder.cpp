#include "AudioAssetBuilder.h"

#include "Sound/SoundWave.h"
#include "AssetToolsModule.h"
#include "Factories/Factory.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Log.h"

// editor only
#if WITH_EDITOR

#include "AudioSynesthesiaNRTFactory.h"
// #include "IAudioAnalyzerInterface.h"
// #include "AudioAnalyzerNRT.h"
// #include "IAudioAnalyzerNRTInterface.h"
#include "LoudnessNRT.h"
#include "OnsetNRT.h"
#include "ConstantQNRT.h"
#include "Sound/SoundSubmix.h"

FGeneratedNRTAssets UAudioAssetBuilder::BuildAllAssets(USoundWave* SourceAudio, const FString& PackagePath)
{
    FGeneratedNRTAssets analysers;

    if (!SourceAudio)
    {
        UE_LOG(LogAudioAnalyzerCore, Log, TEXT("No SourceAudio, building assets cancelled."));
        return analysers;
    }

    FString BaseName = SourceAudio->GetName();

    // TODO: add bools to individually enable packaging certain analyzers.
    analysers.LoudnessNRT = BuildLoudnessAsset(SourceAudio, PackagePath, BaseName + TEXT("_LoudnessNRT"));
    analysers.OnsetNRT = BuildOnsetAsset(SourceAudio, PackagePath, BaseName + TEXT("_OnsetNRT"));
    analysers.ConstantQNRT = BuildConstantQAsset(SourceAudio, PackagePath, BaseName + TEXT("_ConstantQNRT"));

    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Successfully generated all NRT analysers."));
    
    return analysers;
}

ULoudnessNRT* UAudioAssetBuilder::BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName)
{
    if (!SourceAudio)
    {
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No Source Audio present, returning nullptr."));
        return nullptr;
    }

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

    // Create factory
    UAudioSynesthesiaNRTFactory* Factory = NewObject<UAudioSynesthesiaNRTFactory>();
    Factory->AudioSynesthesiaNRTClass = ULoudnessNRT::StaticClass();

    // Create asset
    UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, ULoudnessNRT::StaticClass(), Factory);
    
    ULoudnessNRT* LoudnessAsset = Cast<ULoudnessNRT>(NewAsset);
    if (LoudnessAsset)
    {    
        ULoudnessNRTSettings* Settings = NewObject<ULoudnessNRTSettings>(LoudnessAsset, ULoudnessNRTSettings::StaticClass(), NAME_None, RF_Public | RF_Standalone);
        
        // Create default settings (same settings as default when created manually)
        // CreateDefaultSubobject<ULoudnessNRTSettings>(TEXT("DefaultLoudnessNRTSettings"));
        Settings->AnalysisPeriod = 0.01f; // 100 samples per second
        Settings->MinimumFrequency = 20.0f;
        Settings->MaximumFrequency = 20000.0f;
        Settings->CurveType = ELoudnessNRTCurveTypeEnum::D;
        Settings->NoiseFloorDb = -60.0f;
        
        LoudnessAsset->Sound = SourceAudio;
        LoudnessAsset->Settings = Settings;

        // TODO: move back to editor side and make it work properly ;-;
        // LoudnessAsset->AnalyzeAudio();
        // LoudnessAsset->Analyze(LoudnessAsset->Sound);

        LoudnessAsset->SetFlags(RF_Public | RF_Standalone);

        FAssetRegistryModule::AssetCreated(LoudnessAsset);
        LoudnessAsset->MarkPackageDirty();
    }

    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Successfully created LoudnessNRTAnalyzer"));

    return LoudnessAsset;
}

UOnsetNRT* UAudioAssetBuilder::BuildOnsetAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName)
{
    if (!SourceAudio)
    {
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No Source Audio present, returning nullptr."));
        return nullptr;
    }

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

    // Create factory
    UAudioSynesthesiaNRTFactory* Factory = NewObject<UAudioSynesthesiaNRTFactory>();
    Factory->AudioSynesthesiaNRTClass = UOnsetNRT::StaticClass();

    // Create asset
    UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, UOnsetNRT::StaticClass(), Factory);
    
    UOnsetNRT* OnsetAsset = Cast<UOnsetNRT>(NewAsset);
    if (OnsetAsset)
    {    
        UOnsetNRTSettings* Settings = NewObject<UOnsetNRTSettings>(OnsetAsset, UOnsetNRTSettings::StaticClass(), NAME_None, RF_Public | RF_Standalone);
        
        // Create default settings (same settings as default when created manually)
        // CreateDefaultSubobject<UOnsetNRTSettings>(TEXT("DefaultOnsetNRTSettings"));
        Settings->bDownmixToMono = false;
        Settings->GranularityInSeconds = 0.01f;
        Settings->Sensitivity = 0.5f;
        Settings->MinimumFrequency = 20.0f;
        Settings->MaximumFrequency = 16000.0f;

        OnsetAsset->Sound = SourceAudio;
        OnsetAsset->Settings = Settings;

        // TODO: move back to editor side and make it work properly ;-;
        // OnsetAsset->AnalyzeAudio();
        // OnsetAsset->Analyze(OnsetAsset->Sound);

        OnsetAsset->SetFlags(RF_Public | RF_Standalone);

        FAssetRegistryModule::AssetCreated(OnsetAsset);
        OnsetAsset->MarkPackageDirty();
    }

    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Successfully created OnsetNRTAnalyzer"));

    return OnsetAsset;
}

UConstantQNRT* UAudioAssetBuilder::BuildConstantQAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName)
{
    if (!SourceAudio)
    {
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No Source Audio present, returning nullptr."));
        return nullptr;
    }

    FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

    // Create factory
    UAudioSynesthesiaNRTFactory* Factory = NewObject<UAudioSynesthesiaNRTFactory>();
    Factory->AudioSynesthesiaNRTClass = UConstantQNRT::StaticClass();

    // Create asset
    UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, UConstantQNRT::StaticClass(), Factory);
    
    UConstantQNRT* ConstantQAsset = Cast<UConstantQNRT>(NewAsset);
    if (ConstantQAsset)
    {    
        UConstantQNRTSettings* Settings = NewObject<UConstantQNRTSettings>(ConstantQAsset, UConstantQNRTSettings::StaticClass(), NAME_None, RF_Public | RF_Standalone);
        
        // Create default settings (same settings as default when created manually)
        // CreateDefaultSubobject<UConstantQNRTSettings>(TEXT("DefaultConstantQNRTSettings"));
        Settings->NumBands = 48;
        Settings->NumBandsPerOctave = 12.0f;
        Settings->AnalysisPeriod = 0.01f;
        Settings->bDownmixToMono = false;
        Settings->FFTSize = EConstantQFFTSizeEnum::XLarge;
        Settings->WindowType = EFFTWindowType::Blackman;
        Settings->SpectrumType = EAudioSpectrumType::PowerSpectrum;
        Settings->BandWidthStretch = 1.0f;
        Settings->CQTNormalization = EConstantQNormalizationEnum::EqualEnergy;
        Settings->NoiseFloorDb = -60.0f;

        ConstantQAsset->Sound = SourceAudio;
        ConstantQAsset->Settings = Settings;

        // TODO: move back to editor side and make it work properly ;-;
        // ConstantQAsset->AnalyzeAudio();
        // ConstantQAsset->Analyze(ConstantQAsset->Sound);

        ConstantQAsset->SetFlags(RF_Public | RF_Standalone);

        FAssetRegistryModule::AssetCreated(ConstantQAsset);
        ConstantQAsset->MarkPackageDirty();
    }

    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Successfully created ConstantQNRTAnalyzer"));

    return ConstantQAsset;
}

#endif