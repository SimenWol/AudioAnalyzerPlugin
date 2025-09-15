#pragma once
    
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class AudioAnalyzerEditor : public IModuleInterface
{
public:
    static inline AudioAnalyzerEditor& Get()
    {
        return FModuleManager::LoadModuleChecked<AudioAnalyzerEditor>("AudioAnalyzerEditor");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AudioAnalyzerEditor");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};