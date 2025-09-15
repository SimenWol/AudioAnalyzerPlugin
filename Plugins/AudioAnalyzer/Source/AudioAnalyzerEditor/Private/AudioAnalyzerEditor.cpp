#include "AudioAnalyzerEditor.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

void AudioAnalyzerEditor::StartupModule()
{
    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerEditor module starting up"));
}

void AudioAnalyzerEditor::ShutdownModule()
{
    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerEditor module shutting down"));
}

IMPLEMENT_MODULE(AudioAnalyzerEditor, AudioAnalyzerEditor);