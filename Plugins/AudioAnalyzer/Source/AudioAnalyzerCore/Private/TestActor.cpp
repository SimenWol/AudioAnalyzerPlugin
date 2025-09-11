#include "TestActor.h"

#include "Components/SceneComponent.h"
#include "Components/BillboardComponent.h"

#include "Log.h"

ATestActor::ATestActor(const FObjectInitializer& ObjectInitializer)
{
    SomeProperty = 42;

    RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootComponent"));

    BillboardComponent = ObjectInitializer.CreateDefaultSubobject<UBillboardComponent>(this, TEXT("BillboardComponent"));
    BillboardComponent->SetupAttachment(RootComponent);
}

void ATestActor::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("%s: on BeginPlay, Value is %d"), *GetName(), SomeProperty);
}