#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AudioAnalyzerComponent.generated.h"

// Forward Declarations
class UAudioAnalyzerManager;
class USoundWave;

UCLASS(ClassGroup=(Audio), meta=(BlueprintSpawnableComponent))
class UAudioAnalyzerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAudioAnalyzerComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    USoundWave* SourceAudio;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AudioAnalyzerCore")
    class UAudioAnalyzerManager* AnalyzerManager;

protected:
    virtual void BeginPlay() override;
};