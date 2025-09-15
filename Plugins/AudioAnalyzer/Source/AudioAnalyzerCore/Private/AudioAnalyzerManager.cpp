#include "AudioAnalyzerManager.h"
#include "ConstantQNRT.h"
#include "LoudnessNRT.h"
#include "OnsetNRT.h"

#include "Log.h"

void UAudioAnalyzerManager::InitializeAssets(ULoudnessNRT* Loudness, UOnsetNRT* Onset, UConstantQNRT* ConstantQ)
{
    // TODO: setting variables should become editable

    LoudnessNRT = Loudness;
    OnsetNRT = Onset;
    ConstantQNRT = ConstantQ;
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