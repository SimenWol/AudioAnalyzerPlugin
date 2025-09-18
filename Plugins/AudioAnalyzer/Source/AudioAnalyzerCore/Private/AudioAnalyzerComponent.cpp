#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"

#include "Log.h"

UAudioAnalyzerComponent::UAudioAnalyzerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TimeElapsed = 0.0f;
}

void UAudioAnalyzerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (SourceAudio)
    {
        if (!AnalyzerManager)
        {
            AnalyzerManager = NewObject<UAudioAnalyzerManager>(this);
        }
        
        AnalyzerManager->InitializeAssets(LoudnessNRT, OnsetNRT, ConstantQNRT);
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


// temporary TODO: move to editor module
#if WITH_EDITOR

#include "AudioAssetBuilder.h"

void UAudioAnalyzerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr)
        ? PropertyChangedEvent.Property->GetFName()
        : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, SourceAudio))
    {
        if (SourceAudio)
        {
            // Build new NRT assets
            FString PackagePath = TEXT("/Game/GeneratedAudioAnalysis"); // you can make this configurable
            FGeneratedNRTAssets NewAssets = UAudioAssetBuilder::BuildAllAssets(SourceAudio, PackagePath);

            if (NewAssets.LoudnessNRT == nullptr) {UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("BuildAllAssets returned nullptr for LoudnessNRT"));}

            // Assign to component
            LoudnessNRT   = NewAssets.LoudnessNRT;
            OnsetNRT      = NewAssets.OnsetNRT;
            ConstantQNRT  = NewAssets.ConstantQNRT;

            Modify();
            MarkPackageDirty();
        }
        else
        {
            // If audio was cleared, clear analyzers too
            LoudnessNRT  = nullptr;
            OnsetNRT     = nullptr;
            ConstantQNRT = nullptr;

            Modify();
            MarkPackageDirty();
        }
    }
}
#endif