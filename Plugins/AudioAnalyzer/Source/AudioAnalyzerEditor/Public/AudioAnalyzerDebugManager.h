#pragma once

#include "CoreMinimal.h"
#include "AudioAnalyzerDebugWidget.h"

class UAudioAnalyzerComponent;
class UAudioAnalyzerManager;

class FAudioAnalyzerDebugManager
{
public:
    static FAudioAnalyzerDebugManager& Get();

    void Initialize();
    void Shutdown();

    void OpenDebugWindow(UAudioAnalyzerManager* InManager);
    void CloseDebugWindow();

private:
    FAudioAnalyzerDebugManager() = default;

    TSharedPtr<SWindow> DebugWindow;
    TSharedPtr<SAudioAnalyzerDebugWidget> DebugWidget;
    TWeakObjectPtr<UAudioAnalyzerManager> CurrentManager;
};