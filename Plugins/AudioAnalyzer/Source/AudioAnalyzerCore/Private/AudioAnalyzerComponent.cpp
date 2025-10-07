#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"
#include "Components/AudioComponent.h"

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

    bBeatFiredThisTick = false;

    // Get audio time
    float CurrentTime = 0.0f;

    // TODO: improve this audio time detection system to allow user input / more flexibility; or at least make it more clear that they have to attach an audio component
    UAudioComponent* AudioComp = GetOwner()->FindComponentByClass<UAudioComponent>();
    if (AudioComp /*&& AudioComp->IsPlaying()*/)
    {
        float SoundDuration = AudioComp->Sound->GetDuration();
        CurrentTime = CachedPlaybackPercent * SoundDuration;        
        
        // Reset as audio looped or restarted
        if (CurrentTime < LastTickTime)
        {
            LastBeatTime = -999.0f;
            NextExpectedBeatTime = -999.0f;
            RecentBeatIntervals.Empty();
            RecentOnsetStrengths.Empty();
            TempoConfidence = 0.0f;
        }
    }
    else
    {
        CurrentTime = GetWorld()->GetTimeSeconds();
    }

    ///// Loudness detection /////
    float NewLoudness = AnalyzerManager->GetLoudnessAtTime(CurrentTime);
    if (FMath::Abs(NewLoudness - CachedLoudness) > LoudnessThreshold) // TODO: configurable threshold
    {
        AnalyzerManager->OnLoudnessChanged.Broadcast(NewLoudness);
        CachedLoudness = NewLoudness;
    }

    ///// Onset & beat detection /////
    FOnsetData Onsets = AnalyzerManager->GetOnSetsBetweenTimes(LastTickTime, CurrentTime, 0); // TODO: improve to not be on every tick

    // Update adaptive beat thresholds
    for (float Strength : Onsets.Strengths)
    {
        RecentOnsetStrengths.Add(Strength);
    }
    if (RecentOnsetStrengths.Num() > MaxRecentOnsets)
    {
        RecentOnsetStrengths.RemoveAt(0, RecentOnsetStrengths.Num() - MaxRecentOnsets);
    }

    UpdateAdaptiveThresholds();

    bool HasTempoLock = TempoConfidence > 0.5f && RecentBeatIntervals.Num() >= 3;

    // Check if we've missed a beat
    CheckForMissedBeat(HasTempoLock, CurrentTime);

    // Process onsets for beat detection
    for (int32 i = 0; i < Onsets.Timestamps.Num(); ++i)
    {
        float OnsetTime = Onsets.Timestamps[i];
        float OnsetStrength = Onsets.Strengths[i];
        float OnsetLoudness = AnalyzerManager->GetLoudnessAtTime(OnsetTime); // check

        // Fire Onset event
        AnalyzerManager->OnsetDetected.Broadcast(OnsetTime, OnsetStrength);

        // Fire OnBeat event
        if (IsPotentialBeat(OnsetStrength, OnsetLoudness, OnsetTime))
        {
            AnalyzerManager->OnBeatDetected.Broadcast(OnsetTime);
            UpdateTempoEstimate(OnsetTime);

            LastBeatTime = OnsetTime;
            NextExpectedBeatTime = OnsetTime + BeatInterval;
            bBeatFiredThisTick = true;

            UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Beat Confidence & Estimated BPM: %f | %f"), TempoConfidence, EstimatedBPM);
        }
    }

    // Generate synthetic beat if we missed one
    GenerateSyntheticBeat(HasTempoLock, CurrentTime);

    ///// ConstantQ detection /////
    TArray<float> NewConstantQ = AnalyzerManager->GetConstantQAtTime(CurrentTime, 0); // do we want configurable channels in general?

    if (NewConstantQ.Num() > 0)
    {
        // Check for 'significant' changes in values
        bool bSignificantChange = false;

        if (CachedConstantQ.Num() != NewConstantQ.Num())
        {
            bSignificantChange = true;
        }
        else
        {
            // Compare values with threshold
            for (int32 i = 0; i < NewConstantQ.Num(); ++i)
            {
                if (FMath::Abs(NewConstantQ[i] - CachedConstantQ[i]) > ConstantQThreshold)
                {
                    bSignificantChange = true;
                    break;
                }
            }
        }

        if (bSignificantChange)
        {
            AnalyzerManager->OnConstantQChanged.Broadcast(NewConstantQ, 0);
            CachedConstantQ = NewConstantQ;

            // UE_LOG(LogAudioAnalyzerCore, Log, TEXT("ConstantQ event fired at CurrentTime: %f"), CurrentTime);
        }
    }

    LastTickTime = CurrentTime;
}


// temporary | TODO: move to editor module
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
    // Discard if too close after last beat
    if (OnsetTime - LastBeatTime < MinBeatInterval)
    {
        return false;
    }

    // Check if we have an established tempo
    bool HasTempoLock = TempoConfidence > 0.5f && RecentBeatIntervals.Num() >= 3;
    if (HasTempoLock)
    {
        // Be more selective with tempo lock
        float TimeSinceExpected = FMath::Abs(OnsetTime - NextExpectedBeatTime);

        // Check if we might be at a multiple of the beat interval
        float NearestBeatMultiple = FMath::RoundToFloat(TimeSinceExpected / BeatInterval) * BeatInterval;
        float DistanceToNearestBeat = FMath::Abs(TimeSinceExpected - NearestBeatMultiple);

        bool IsNearExpectedTime = DistanceToNearestBeat < BeatTimingTolerance;

        // If near expected time / beat, lower thresholds
        if (IsNearExpectedTime)
        {
            return OnsetStrength > AdaptiveOnsetThreshold * 0.7f && OnsetLoudness > AdaptiveLoudnessThreshold * 0.7f;
        }
        else
        {
            // Off-beat needs to be much stronger to register
            return OnsetStrength > AdaptiveOnsetThreshold * 1.3f && OnsetLoudness > AdaptiveLoudnessThreshold * 1.2f;
        }
    }
    else
    {
        // No established tempo - use standard thresholds to establish a tempo
        return OnsetStrength > AdaptiveOnsetThreshold && OnsetLoudness > AdaptiveLoudnessThreshold;
    }
}

void UAudioAnalyzerComponent::UpdateAdaptiveThresholds()
{
    if (RecentOnsetStrengths.Num() < 5) { return; }

    // Calculate median and mean for robust threshold
    TArray<float> SortedStrengths = RecentOnsetStrengths;
    SortedStrengths.Sort();

    float Median = SortedStrengths[SortedStrengths.Num() / 2];
    float Sum = 0.0f;
    for (float Strength : SortedStrengths) { Sum += Strength; }
    float Mean = Sum / SortedStrengths.Num();

    // Threshold is 60%+ between median / max
    float Max = SortedStrengths.Last();
    AdaptiveOnsetThreshold = FMath::Lerp(Median, Max, 0.6f);

    // Clamp to keep reasonable bounds
    AdaptiveOnsetThreshold = FMath::Clamp(AdaptiveOnsetThreshold, 0.2f, 0.8f);
}

void UAudioAnalyzerComponent::UpdateTempoEstimate(float BeatTime)
{
    if (LastBeatTime > 0.0f)
    {
        float Interval = BeatTime - LastBeatTime;

        // Only use reasonable intervals
        if (Interval >= MinBeatInterval && Interval <= MaxBeatInterval)
        {
            RecentBeatIntervals.Add(Interval);

            if (RecentBeatIntervals.Num() > MaxRecentBeats)
            {
                RecentBeatIntervals.RemoveAt(0);
            }

            // Calculate median interval
            if (RecentBeatIntervals.Num() >= 3)
            {
                TArray<float> SortedIntervals = RecentBeatIntervals;
                SortedIntervals.Sort();

                float MedianInterval = SortedIntervals[SortedIntervals.Num() / 2];
                BeatInterval = MedianInterval;
                EstimatedBPM = 60.0f / MedianInterval;

                // Increase confidence with more beats
                TempoConfidence = FMath::Min(1.0f, RecentBeatIntervals.Num() / 6.0f);
            }
        }
    }
}

void UAudioAnalyzerComponent::GenerateSyntheticBeat(bool bHasTempoLock, float CurrentTime)
{
    if (bHasTempoLock && !bBeatFiredThisTick && bEnableSyntheticBeats
        && TempoConfidence >= MinConfidenceForSyntheticBeats && NextExpectedBeatTime > 0.0f)
    {
        float TimeSinceExpected = CurrentTime - NextExpectedBeatTime;

        // If past expect beat time by small margin -> fire synthetic beat + decrease confidence
        if (TimeSinceExpected > 0.0f && TimeSinceExpected < BeatTimingTolerance * 1.5f)
        {
            AnalyzerManager->OnBeatDetected.Broadcast(NextExpectedBeatTime);
            LastBeatTime = NextExpectedBeatTime;
            NextExpectedBeatTime += BeatInterval;

            TempoConfidence *= SyntheticBeatConfidenceDecay;

            UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Synthetic Beat | Confidence: %f | BPM: %f"), TempoConfidence, EstimatedBPM);
        }
    }
}

void UAudioAnalyzerComponent::CheckForMissedBeat(bool bHasTempoLock, float CurrentTime)
{
    if (bHasTempoLock && LastBeatTime > 0.0f)
    {
        float TimeSinceLastBeat = CurrentTime - LastBeatTime;
        float MaxExpectedGap = BeatInterval * 1.5f;

        if (TimeSinceLastBeat > MaxExpectedGap)
        {
            // We've missed beat(s) - resync
            int32 MissedBeats = FMath::RoundToInt(TimeSinceLastBeat / BeatInterval);
            NextExpectedBeatTime = LastBeatTime + (MissedBeats * BeatInterval);

            // Reduce confidence slightly but don't reset entirely
            TempoConfidence = FMath::Max(0.3f, TempoConfidence * 0.8f);
        }
    }
}

void UAudioAnalyzerComponent::OnPlaybackPercentChanged(const USoundWave* PlayingSoundWave, float PlaybackPercent)
{
    CachedPlaybackPercent = PlaybackPercent;
}