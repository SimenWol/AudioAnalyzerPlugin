#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"
#include "Components/AudioComponent.h"

#include "Log.h"

UAudioAnalyzerComponent::UAudioAnalyzerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    // TimeElapsed = 0.0f;
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

        UAudioComponent* AudioComp = GetOwner()->FindComponentByClass<UAudioComponent>();
        if (AudioComp)
        {
            AudioComp->OnAudioPlaybackPercent.AddDynamic(this, &UAudioAnalyzerComponent::OnPlaybackPercentChanged);
        }
    }
}

// temporary test function
void UAudioAnalyzerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // TODO: maybe schedule events as data is precomputed?
    if (!AnalyzerManager || !SourceAudio) return;

    // Get audio time
    float CurrentTime = 0.0f;

    UAudioComponent* AudioComp = GetOwner()->FindComponentByClass<UAudioComponent>();
    if (AudioComp /*&& AudioComp->IsPlaying()*/)
    {
        float SoundDuration = AudioComp->Sound->GetDuration();
        CurrentTime = CachedPlaybackPercent * SoundDuration;
        
        
        if (CurrentTime < LastTickTime)
        {
            LastBeatTime = -999.0f; // Reset lastbeattime
        }
    }
    else
    {
        CurrentTime = GetWorld()->GetTimeSeconds();
    }

    // Loudness detection
    float NewLoudness = AnalyzerManager->GetLoudnessAtTime(CurrentTime);
    if (FMath::Abs(NewLoudness - CachedLoudness) > LoudnessThreshold) // TODO: configurable threshold
    {
        AnalyzerManager->OnLoudnessChanged.Broadcast(NewLoudness);
        CachedLoudness = NewLoudness;
    }

    // Onset & beat detection
    FOnsetData Onsets = AnalyzerManager->GetOnSetsBetweenTimes(LastTickTime, CurrentTime, 0); // TODO: improve to not be on every tick

    for (int32 i = 0; i < Onsets.Timestamps.Num(); ++i)
    {
        float OnsetTime = Onsets.Timestamps[i];
        float OnsetStrength = Onsets.Strengths[i];
        float OnsetLoudness = AnalyzerManager->GetLoudnessAtTime(OnsetTime); // check

        // Fire Onset event
        AnalyzerManager->OnsetDetected.Broadcast(OnsetTime, OnsetStrength);

        // temp/wip beat detection
        if (IsPotentialBeat(OnsetStrength, OnsetLoudness, OnsetTime))
        {
            AnalyzerManager->OnBeatDetected.Broadcast(OnsetTime);
            LastBeatTime = OnsetTime;
        }
    }

    LastTickTime = CurrentTime;
}


// temporary TODO: move to editor module
#if WITH_EDITOR

#include "AudioAssetBuilder.h"

void UAudioAnalyzerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, SourceAudio))
    {
        if (SourceAudio)
        {
            // Build new NRT assets
            FString PackagePath = TEXT("/Game/GeneratedAudioAnalysis"); // you can make this configurable
            FGeneratedNRTAssets NewAssets = UAudioAssetBuilder::BuildAllAssets(SourceAudio, PackagePath);

            if (NewAssets.LoudnessNRT == nullptr) { UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("BuildAllAssets returned nullptr for LoudnessNRT")); }

            // Attaching assets on component still broken. TODO: revisit & fix.
            if (NewAssets.LoudnessNRT)
            {
                Modify(); // record transaction on *this* component
                LoudnessNRT   = NewAssets.LoudnessNRT;
                OnsetNRT      = NewAssets.OnsetNRT;
                ConstantQNRT  = NewAssets.ConstantQNRT;

                if (UObject* Outer = GetOuter())
                {
                    Outer->MarkPackageDirty();
                }

                // Trigger editor refresh of the property field
                FProperty* ChangedProp = FindFProperty<FProperty>(UAudioAnalyzerComponent::StaticClass(),
                    GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, LoudnessNRT));

                if (ChangedProp)
                {
                    FPropertyChangedEvent ChangedEvent(ChangedProp);
                    PostEditChangeProperty(ChangedEvent);
                }

                UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Assigned LoudnessNRT %s"), *LoudnessNRT->GetPathName());

                if (LoudnessNRT)
                {
                    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Final Assigned LoudnessNRT Path: %s"), *LoudnessNRT->GetPathName());
                }
            }
        }
        else
        {
            // If audio was cleared, clear analyzers too
            LoudnessNRT  = nullptr;
            OnsetNRT     = nullptr;
            ConstantQNRT = nullptr;

            if (AActor* Owner = GetOwner())
            {
                Owner->Modify();
                Owner->MarkPackageDirty();
            }
        }
    }
}
#endif

bool UAudioAnalyzerComponent::IsPotentialBeat(float OnsetStrength, float OnsetLoudness, float OnsetTime) const
{
    if (OnsetTime - LastBeatTime < MinBeatInterval
        || OnsetStrength < OnsetStrengthThreshold
        || OnsetLoudness < BeatLoudnessThreshold)
    {
        return false;
    }

    return true;
}

void UAudioAnalyzerComponent::OnPlaybackPercentChanged(const USoundWave* PlayingSoundWave, float PlaybackPercent)
{
    CachedPlaybackPercent = PlaybackPercent;
}