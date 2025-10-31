#include "AudioAnalyzerDebugManager.h"
#include "AudioAnalyzerManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "Log.h"

FAudioAnalyzerDebugManager& FAudioAnalyzerDebugManager::Get()
{
    static FAudioAnalyzerDebugManager Instance;
    return Instance;
}

void FAudioAnalyzerDebugManager::Initialize()
{
    // Empty for now
}

void FAudioAnalyzerDebugManager::Shutdown()
{
    CloseDebugWindow();
}

void FAudioAnalyzerDebugManager::OpenDebugWindow(UAudioAnalyzerManager* InManager)
{
    if (!InManager)
    {
        UE_LOG(LogAudioAnalyzerEditor, Warning, TEXT("Cannot open debug window: No AudioAnalyzerManager provided."));
        return;
    }

    if (DebugWindow.IsValid() && CurrentManager == InManager)
    {
        DebugWindow->BringToFront();
        return;
    }
    else
    {
        // Create new window
        CurrentManager = InManager;

        DebugWidget = SNew(SAudioAnalyzerDebugWidget);
        DebugWidget->SetAnalyzerManager(InManager);

        DebugWindow = SNew(SWindow)
            .Title(FText::FromString("Audio Analyzer Debug"))
            .ClientSize(FVector2D(500, 600))
            .SupportsMaximize(true)
            .SupportsMinimize(true)
            [
                DebugWidget.ToSharedRef()
            ];

        // Add window to Slate application
        if (FSlateApplication::IsInitialized())
        {
            FSlateApplication::Get().AddWindow(DebugWindow.ToSharedRef());
        }
    }
}

void FAudioAnalyzerDebugManager::CloseDebugWindow()
{
    if (DebugWindow.IsValid())
    {
        DebugWindow->RequestDestroyWindow();
        DebugWindow.Reset();
        DebugWidget.Reset();
        CurrentManager.Reset();
    }
}