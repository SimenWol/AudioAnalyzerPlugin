#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AudioAnalyzerDebugLibrary.generated.h"

class UAudioAnalyzerManager;

UCLASS()
class UAudioAnalyzerDebugLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Opens the Audio Analyzer debug window for the given manager. */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzer|Debug", meta=(DevelopmentOnly))
    static void OpenAudioAnalyzerDebugWindow(UAudioAnalyzerManager* Manager);

    /** Closes the Audio Analyzer debug window. */
    UFUNCTION(BlueprintCallable, Category="AudioAnalyzer|Debug", meta=(DevelopmentOnly))
    static void CloseAudioAnalyzerDebugWindow();
};