#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAnalyzerManager.generated.h"

// Forward Declarations
class UConstantQNRT;
class ULoudnessNRT;
class UOnsetNRT;

UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void InitializeAssets(ULoudnessNRT* Loudness, UOnsetNRT* OnSet, UConstantQNRT* ConstantQ);

    // Test function
    float GetLoudnessAtTime(float TimeSeconds) const;
    // float GetOnSetAtTime(float TimeSeconds, float Strength) const;

private:
    UPROPERTY()
    UConstantQNRT* ConstantQNRT;

    UPROPERTY()
    ULoudnessNRT* LoudnessNRT;

    UPROPERTY()
    UOnsetNRT* OnsetNRT;
};