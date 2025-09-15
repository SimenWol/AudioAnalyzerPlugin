#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"

#include "Log.h"

UAudioAnalyzerComponent::UAudioAnalyzerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAudioAnalyzerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (SourceAudio)
    {
        AnalyzerManager = NewObject<UAudioAnalyzerManager>(this);
        AnalyzerManager->Initialize(SourceAudio);
    }
}

// temporary test function
void UAudioAnalyzerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeElapsed += DeltaTime;

    if (AnalyzerManager)
    {
        float Loudness = AnalyzerManager->GetLoudnessAtTime(TimeElapsed);
        UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Loudness at %.2f = %.3f"), TimeElapsed, Loudness);
    }
}