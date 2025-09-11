#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"

UAudioAnalyzerComponent::UAudioAnalyzerComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // No need to be enabled for now as it's NRT
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