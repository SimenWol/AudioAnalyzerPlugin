#include "AudioAnalyzerDebugLibrary.h"
#include "AudioAnalyzerDebugManager.h"
#include "AudioAnalyzerManager.h"
#include "Log.h"

void UAudioAnalyzerDebugLibrary::OpenAudioAnalyzerDebugWindow(UAudioAnalyzerManager* Manager)
{
    if (!Manager)
    {
        UE_LOG(LogAudioAnalyzerEditor, Warning, TEXT("Cannot open Audio Analyzer Debug Window: Manager is null."));
        return;
    }

    FAudioAnalyzerDebugManager::Get().OpenDebugWindow(Manager);
}

void UAudioAnalyzerDebugLibrary::CloseAudioAnalyzerDebugWindow()
{
    FAudioAnalyzerDebugManager::Get().CloseDebugWindow();
}