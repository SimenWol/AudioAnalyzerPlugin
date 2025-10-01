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
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No LoudnessNRT, returned 0.0f"));
        return 0.0f;
    }
    
    float Result;
    LoudnessNRT->GetNormalizedLoudnessAtTime(TimeSeconds, Result);
    
    //UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Normalized Loudness Result: %f"), Result);
    
    return Result;
}

TArray<float> UAudioAnalyzerManager::GetConstantQAtTime(float TimeSeconds, int32 ChannelIndex) const
{
    TArray<float> OutConstantQ;

    if (!ConstantQNRT)
    {
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No ConstantQNRT, returning empty array"));
        return OutConstantQ;
    }

    ConstantQNRT->GetNormalizedChannelConstantQAtTime(TimeSeconds, ChannelIndex, OutConstantQ);

    // UE_LOG(LogAudioAnalyzerCore, Log, TEXT("ConstantQ[%d] at %f sec: %d bins"),
    //         ChannelIndex, TimeSeconds, OutConstantQ.Num());
    
    return OutConstantQ;
}

FOnsetData UAudioAnalyzerManager::GetOnSetsBetweenTimes(float StartSeconds, float EndSeconds, int32 ChannelIndex) const
{
    FOnsetData Result;

    if (!OnsetNRT)
    {
        UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("No OnsetNRT, returning empty onset data"));
        return Result;
    }

    OnsetNRT->GetNormalizedChannelOnsetsBetweenTimes(StartSeconds, EndSeconds, ChannelIndex, Result.Timestamps, Result.Strengths);

    // UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Onsets found: %d between %f and %f (channel %d)"),
    //         Result.Timestamps.Num(), StartSeconds, EndSeconds, ChannelIndex);

    return Result;
}