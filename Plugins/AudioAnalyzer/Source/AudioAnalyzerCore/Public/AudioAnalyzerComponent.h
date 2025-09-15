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
class UAudioAnalyzerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAudioAnalyzerComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    USoundWave* SourceAudio;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UConstantQNRT* ConstantQNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    ULoudnessNRT* LoudnessNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    UOnsetNRT* OnsetNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    class UAudioAnalyzerManager* AnalyzerManager;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // temp test function

private:
    float TimeElapsed;
};