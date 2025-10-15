#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AudioAnalyzerDebugProxy.generated.h"

class SAudioAnalyzerDebugWidget;

UCLASS()
class UAudioAnalyzerDebugProxy : public UObject
{
    GENERATED_BODY()

public:
    void SetWidget(TSharedPtr<SAudioAnalyzerDebugWidget> InWidget);

    UFUNCTION()
    void OnLoudnessChanged(float Loudness);

    UFUNCTION()
    void OnOnsetDetected(float TimeSeconds, float Strength);

    UFUNCTION()
    void OnConstantQChanged(const TArray<float>& Values, int32 ChannelIndex);

    UFUNCTION()
    void OnBeatDetected(float TimeSeconds);

private:
    TWeakPtr<SAudioAnalyzerDebugWidget> Widget;
};