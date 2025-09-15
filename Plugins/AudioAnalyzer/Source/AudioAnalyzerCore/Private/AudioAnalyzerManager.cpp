#include "AudioAnalyzerManager.h"
#include "ConstantQNRT.h"
#include "LoudnessNRT.h"
#include "OnSetNRT.h"
#include "Sound/SoundWave.h"

#include "Log.h"

void UAudioAnalyzerManager::Initialize(USoundWave* Source)
{
    // TODO: just realised this is going to require a different 'manager' object *per* soundwave and/or actor. maybe not the best solution?
    // temp test, variables should become editable
    if (!Source) return;

    ConstantQNRT = NewObject<UConstantQNRT>(this);
    UConstantQNRTSettings* CQSettings = NewObject<UConstantQNRTSettings>();
    
    ConstantQNRT->Sound = Source;
    ConstantQNRT->Settings = CQSettings;
    CQSettings->AnalysisPeriod = 0.01f; // 100 samples per second
    CQSettings->NumBands = 96;

    LoudnessNRT = NewObject<ULoudnessNRT>(this);
    ULoudnessNRTSettings* LoudnessSettings = NewObject<ULoudnessNRTSettings>();
    
    LoudnessNRT->Sound = Source;
    LoudnessNRT->Settings = LoudnessSettings;
    LoudnessSettings->AnalysisPeriod = 0.01f; // 100 samples per second
    LoudnessSettings->CurveType = ELoudnessNRTCurveTypeEnum::A;
    
    OnsetNRT = NewObject<UOnsetNRT>(this);
    UOnsetNRTSettings* OnsetSettings = NewObject<UOnsetNRTSettings>();
    OnsetNRT->Sound = Source;
    OnsetNRT->Settings = OnsetSettings;
    OnsetSettings->Sensitivity = 0.5f;
}

float UAudioAnalyzerManager::GetLoudnessAtTime(float TimeSeconds) const
{
    if (!LoudnessNRT)
    {
        UE_LOG(LogAudioAnalyzerCore, Log, TEXT("No LoudnessNRT, returned 0.0f"));
        return 0.0f;
    }
    
    float Result;
    LoudnessNRT->GetNormalizedLoudnessAtTime(TimeSeconds, Result);
    
    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("%f"), Result);
    return Result;
}

// float UAudioAnalyzerManager::GetOnSetAtTime(float TimeSeconds, float Strength) const
// {
//     if (!OnsetNRT)
//     {
//         UE_LOG(LogAudioAnalyzerCore, Log, TEXT("No OnsetNRT, returned 0.0f"));
//         return 0.0f;
//     }

//     float Result;
//     OnsetNRT->GetNormalizedOnsetsAtTime();
// }