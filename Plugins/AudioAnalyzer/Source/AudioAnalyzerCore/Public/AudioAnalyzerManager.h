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

UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void InitializeAssets(ULoudnessNRT* Loudness, UOnsetNRT* OnSet, UConstantQNRT* ConstantQ);

public:
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    float GetLoudnessAtTime(float TimeSeconds) const;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    TArray<float> GetConstantQAtTime(float TimeSeconds, int32 ChannelIndex) const;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    FOnsetData GetOnSetsBetweenTimes(float StartSeconds, float EndSeconds, int32 ChannelIndex) const;

private:
    UPROPERTY()
    TObjectPtr<UConstantQNRT> ConstantQNRT;

    UPROPERTY()
    TObjectPtr<ULoudnessNRT> LoudnessNRT;

    UPROPERTY()
    TObjectPtr<UOnsetNRT> OnsetNRT;
};