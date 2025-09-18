#include "AudioAnalyzerEditor.h"
#include "AudioAnalyzerComponentCustomization.h"

#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

void AudioAnalyzerEditor::StartupModule()
{
    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerEditor module starting up"));

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerEditor module has started"));
}

void AudioAnalyzerEditor::ShutdownModule()
{
    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerEditor module shutting down"));

    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    }
}

IMPLEMENT_MODULE(AudioAnalyzerEditor, AudioAnalyzerEditor);