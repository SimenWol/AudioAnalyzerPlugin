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
    float TimeElapsed;
};