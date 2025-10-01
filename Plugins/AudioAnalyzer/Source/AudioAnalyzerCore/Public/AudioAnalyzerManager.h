#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAnalyzerManager.generated.h"

// Forward Declarations
class UConstantQNRT;
class ULoudnessNRT;
class UOnsetNRT;

USTRUCT(BlueprintType)
struct FOnsetData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Timestamps;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Strengths;
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoudnessChangedSignature, float, Loudness);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeatDetectedSignature, float, TimeSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpectrumBandChangedSignature, int32, BandIndex, float, Magnitude);

UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void InitializeAssets(ULoudnessNRT* Loudness, UOnsetNRT* OnSet, UConstantQNRT* ConstantQ);

public:
    // Functions
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    float GetLoudnessAtTime(float TimeSeconds) const;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    TArray<float> GetConstantQAtTime(float TimeSeconds, int32 ChannelIndex) const;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    FOnsetData GetOnSetsBetweenTimes(float StartSeconds, float EndSeconds, int32 ChannelIndex) const;

public:
    // Events
    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnLoudnessChangedSignature OnLoudnessChanged;

    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnBeatDetectedSignature OnBeatDetected;

    UPROPERTY(BlueprintAssignable, Category="AudioAnalyzerCore|Events")
    FOnSpectrumBandChangedSignature OnSpectrumBandChanged;

private:
    UPROPERTY()
    TObjectPtr<UConstantQNRT> ConstantQNRT;

    UPROPERTY()
    TObjectPtr<ULoudnessNRT> LoudnessNRT;

    UPROPERTY()
    TObjectPtr<UOnsetNRT> OnsetNRT;
};