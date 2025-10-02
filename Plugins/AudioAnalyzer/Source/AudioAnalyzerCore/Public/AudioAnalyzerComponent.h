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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    TObjectPtr<USoundWave> SourceAudio;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.ConstantQNRT"))
    TObjectPtr<UConstantQNRT> ConstantQNRT;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.LoudnessNRT"))
    TObjectPtr<ULoudnessNRT> LoudnessNRT;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore", meta=(AllowedClasses="/Script/AudioSynesthesia.OnsetNRT"))
    TObjectPtr<UOnsetNRT> OnsetNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    TObjectPtr<UAudioAnalyzerManager> AnalyzerManager;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // temp test function

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    bool IsPotentialBeat(float OnsetStrength, float OnsetLoudness, float OnsetTime) const;

private:

    // Loudness tracking
    float CachedLoudness = 0.0f;
    float LoudnessThreshold = 0.05f;
    
    // Beat detection
    float LastTickTime = 0.0f;
    float LastBeatTime = -999.0f;

public:
    // Beat detection parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    float MinBeatInterval = 0.3f; // limits to ~200 BPM to avoid double-triggering. lowering theshold allows to go to higher BPM but may create double triggers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    float OnsetStrengthThreshold = 0.3f; // Minimum OnSet strength to consider
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    float BeatLoudnessThreshold = 0.15f; // Minimum loudness for a beat
};