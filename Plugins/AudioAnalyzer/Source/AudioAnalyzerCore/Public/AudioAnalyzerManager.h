#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAnalyzerManager.generated.h"

// Forward Declerations
class UConstantQNRT;
class ULoudnessNRT;
class UOnsetNRT;
// class USoundWave;

UCLASS(BlueprintType)
class UAudioAnalyzerManager : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UConstantQNRT* ConstantQNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    ULoudnessNRT* LoudnessNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UOnsetNRT* OnsetNRT;

    UFUNCTION(BlueprintCallable, Category="AudioAnalyzerCore")
    void Initialize(USoundWave* Source);

    // Test function
    float GetLoudnessAtTime(float TimeSeconds) const;
};