#include "AudioAnalyzerManager.h"
#include "ConstantQNRT.h"
#include "LoudnessNRT.h"
#include "OnSetNRT.h"
#include "Sound/SoundWave.h"

void UAudioAnalyzerManager::Initialize(USoundWave* Source)
{
    if (!Source) return;

    ConstantQNRT = NewObject<UConstantQNRT>(this);
    ConstantQNRT->Sound = Source;

    LoudnessNRT = NewObject<ULoudnessNRT>(this);
    LoudnessNRT->Sound = Source;

    OnsetNRT = NewObject<UOnsetNRT>(this);
    OnsetNRT->Sound = Source;
}

float UAudioAnalyzerManager::GetLoudnessAtTime(float TimeSeconds) const
{
    if (!LoudnessNRT) return 0.0f;
    
    float Result;
    LoudnessNRT->GetNormalizedLoudnessAtTime(TimeSeconds, Result);
    
    return Result;
}