#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ConstantQNRT.h"
#include "LoudnessNRT.h"
#include "OnsetNRT.h"

#include "AudioAnalyzerComponent.generated.h"

// Forward Declarations
class UAudioAnalyzerManager;
class USoundWave;
class UConstantQNRT;
class ULoudnessNRT;
class UOnsetNRT;

UCLASS(ClassGroup=(Audio), meta=(BlueprintSpawnableComponent))
class AUDIOANALYZERCORE_API UAudioAnalyzerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAudioAnalyzerComponent();

    /** The SourceAudio that should be analyzed by the audio analyzer component. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    TObjectPtr<USoundWave> SourceAudio;

    /** The generated ConstantQ analysis data. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.ConstantQNRT"))
    TObjectPtr<UConstantQNRT> ConstantQNRT;
    
    /** The generated Loudness analysis data. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.LoudnessNRT"))
    TObjectPtr<ULoudnessNRT> LoudnessNRT;
    
    /** The generated Onset analysis data. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.OnsetNRT"))
    TObjectPtr<UOnsetNRT> OnsetNRT;

    /** The analyzer manager attached to this analysis component. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    TObjectPtr<UAudioAnalyzerManager> AnalyzerManager;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // temp test function

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    /** Analyzes the provided Onset information to detect a potential beat. */
    bool IsPotentialBeat(float OnsetStrength, float OnsetLoudness, float OnsetTime) const;
    /** Updates the adaptive beat thresholds for more accurate beat / tempo tracking. */
    void UpdateAdaptiveThresholds();
    /** Uses current information from onsets and (adaptive) thresholds to update tempo estimates for beat tracking. */
    void UpdateTempoEstimate(float BeatTime);
    /** Generates synthetic beat events in the case of missed beats in the music. */
    void GenerateSyntheticBeat(bool bHasTempoLock, float CurrentTime);
    /** Checks for any skipped beats and updates the next potential beat prediction accordingly. */
    void CheckForMissedBeat(bool bHasTempoLock, float CurrentTime);

    UFUNCTION()
    void OnPlaybackPercentChanged(const USoundWave* PlayingSoundWave, float PlaybackPercent);

private:
    // Cached data
    float CachedPlaybackPercent = 0.0f;
    float CachedLoudness = 0.0f;
    TArray<float> CachedConstantQ;
    
    // Beat detection //
    float LastTickTime = 0.0f;
    float LastBeatTime = -999.0f;
    float NextExpectedBeatTime = -999.0f;

    // Adaptive thresholds
    TArray<float> RecentOnsetStrengths;
    TArray<float> RecentBeatIntervals;
    float AdaptiveOnsetThreshold = 0.3f;
    float AdaptiveLoudnessThreshold = 0.15f;

    // Tempo tracking
    float EstimatedBPM = 120.0f; // Assume standard 120 bpm to start. | TODO: let designer set BPM / beat timing for non-detection option.
    float BeatInterval = 60.0f / EstimatedBPM;
    float TempoConfidence = 0.0f;

    // Synthetic beat events
    bool bBeatFiredThisTick = false;

    // Detection parameters
    int32 MaxRecentBeats = 8; // Amount of recent beats saved for tempo estimation.
    int32 MaxRecentOnsets = 50; // Amount of recent Onset strengths saved for tempo estimation.
    
public:
    /** Amount that loudness needs to change with before firing another OnLoudness event. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|Loudness")
    float LoudnessThreshold = 0.05f;
    /** Amount that any ConstantQ band needs to change with before firing another OnConstantQ event. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|ConstantQ")
    float ConstantQThreshold = 0.1f;

    /** How far off expected beat timing we allow an OnBeat event to trigger (in seconds). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    float BeatTimingTolerance = 0.12f;
    /** Minimum interval between beats (the lower the interval, the higher BPM is allowed but also introduces more margin for error). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    float MinBeatInterval = 0.25f;
    /** Maximum interval between beats (the higher the interval, the lower BPM is allowed but also introduces more margin for error). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    float MaxBeatInterval = 1.5f;

    /** Whether the analyzer should produce artificial beats when it cannot detect a beat but has detected a tempo. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    bool bEnableSyntheticBeats = true;
    /** The minimum amount of confidence in tempo needed in order for a synthetic beat to be produced. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    float MinConfidenceForSyntheticBeats = 0.6f;
    /** The amount by which the tempo confidence is multiplied each time a synthetic beat is produced. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore|BeatTracking")
    float SyntheticBeatConfidenceDecay = 0.95f;
};