#pragma once
    
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class AudioAnalyzerCore : public IModuleInterface
{
public:
    static inline AudioAnalyzerCore& Get()
    {
        return FModuleManager::LoadModuleChecked<AudioAnalyzerCore>("AudioAnalyzerCore");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("AudioAnalyzerCore");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};