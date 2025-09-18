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
    USoundWave* SourceAudio;

    UPROPERTY(VisibleAnywhere, Instanced, Category="AudioAnalyzerCore")
    UConstantQNRT* ConstantQNRT;

    UPROPERTY(VisibleAnywhere, Instanced, Category="AudioAnalyzerCore")
    ULoudnessNRT* LoudnessNRT;

    UPROPERTY(VisibleAnywhere, Instanced, Category="AudioAnalyzerCore")
    UOnsetNRT* OnsetNRT;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    class UAudioAnalyzerManager* AnalyzerManager;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; // temp test function

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    float TimeElapsed;
};