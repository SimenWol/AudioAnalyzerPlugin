#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAnalyzerManager.generated.h"

// Forward Declerations
class UConstantQNRT;
// class UConstantQNRTSettings;
class ULoudnessNRT;
// class ULoudnessNRTSettings;
class UOnsetNRT;
// class UOnsetNRTSettings;
// class USoundWave;

UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UConstantQNRT* ConstantQNRT;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    // UConstantQNRTSettings* QNRTSettings;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    ULoudnessNRT* LoudnessNRT;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    // ULoudnessNRTSettings* LoudnessNRTSettings;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UOnsetNRT* OnsetNRT;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    // UOnsetNRTSettings* OnsetNRTSettings;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void Initialize(USoundWave* Source);

    // Test function
    float GetLoudnessAtTime(float TimeSeconds) const;
    // float GetOnSetAtTime(float TimeSeconds, float Strength) const;
};