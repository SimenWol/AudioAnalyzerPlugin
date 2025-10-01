#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TestActor.generated.h"

UCLASS()
class ATestActor : public AActor
{
    GENERATED_BODY()

public:
    ATestActor(const FObjectInitializer& ObjectInitializer);

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    class UBillboardComponent* BillboardComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AudioAnalyzerCore")
    int32 SomeProperty;

protected:
    void BeginPlay() override;

};