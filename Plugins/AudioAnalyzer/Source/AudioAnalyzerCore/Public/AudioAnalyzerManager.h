#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAnalyzerManager.generated.h"

// Forward Declarations
class UConstantQNRT;
class ULoudnessNRT;
class UOnsetNRT;

/** A struct containing data about detected Onsets in audio. */
USTRUCT(BlueprintType)
struct FOnsetData
{
    GENERATED_BODY()

    /** Timestamps at which these detected Onsets occur. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Timestamps;

    /** Strengths of the detected Onsets. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Strengths;
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoudnessChangedSignature, float, Loudness);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnsetDetectedSignature, float, TimeSeconds, float, OnsetStrength);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectrumBandChangedSignature, int32, BandIndex, float, Magnitude);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeatDetectedSignature, float, TimeSeconds);

/** A class that serves as a simple bridge / API between different modules and the audio analysis component. */
UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    /** Initialize analysis data to be used by the analysis functions. */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void InitializeAssets(ULoudnessNRT* Loudness, UOnsetNRT* OnSet, UConstantQNRT* ConstantQ);

public:
    /** Returns the loudness of the selected audio at the provided time. */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    float GetLoudnessAtTime(float TimeSeconds) const;

    /** Returns list of envelopes containing ConstantQ data of the selected audio at the provided time.  */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    TArray<float> GetConstantQAtTime(float TimeSeconds, int32 ChannelIndex) const;

    /** Returns list of Onset timestamps & strengths of the selected audio between the provided times. */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    FOnsetData GetOnSetsBetweenTimes(float StartSeconds, float EndSeconds, int32 ChannelIndex) const;

public:
    /** Event that triggers whenever a change in loudness has been detected on the selected audio. */
    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnLoudnessChangedSignature OnLoudnessChanged;

    /** Event that triggers whenever an Onset has been detected on the selected audio. */
    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnsetDetectedSignature OnsetDetected;

    /** TBA */
    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnSpectrumBandChangedSignature OnSpectrumBandChanged;

    /** Event that triggers each time a beat has been detected on the selected audio. */
    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnBeatDetectedSignature OnBeatDetected;

private:
    UPROPERTY()
    TObjectPtr<UConstantQNRT> ConstantQNRT;

    UPROPERTY()
    TObjectPtr<ULoudnessNRT> LoudnessNRT;

    UPROPERTY()
    TObjectPtr<UOnsetNRT> OnsetNRT;
};