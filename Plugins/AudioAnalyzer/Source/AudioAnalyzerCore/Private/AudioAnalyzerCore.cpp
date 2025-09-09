#include "AudioAnalyzerCore.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

void AudioAnalyzerCore::StartupModule()
{
    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("AudioAnalyzerCore module starting up"));
}

void AudioAnalyzerCore::ShutdownModule()
{
    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("AudioAnalyzerCore module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(AudioAnalyzerCore, AudioAnalyzerCore, "AudioAnalyzerCore");