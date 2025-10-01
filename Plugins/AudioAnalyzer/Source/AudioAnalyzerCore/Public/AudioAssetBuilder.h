#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

// Forward Declarations
class USoundWave;
class ULoudnessNRT;
class UOnsetNRT;
class UConstantQNRT;

struct FGeneratedNRTAssets
{
    TObjectPtr<ULoudnessNRT> LoudnessNRT = nullptr;
    TObjectPtr<UOnsetNRT> OnsetNRT = nullptr;
    TObjectPtr<UConstantQNRT> ConstantQNRT = nullptr;
};

class UAudioAssetBuilder : public UObject
{
public:
    static FGeneratedNRTAssets BuildAllAssets(USoundWave* SourceAudio, const FString& PackagePath);

private:
    // Builders
    static ULoudnessNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    static UOnsetNRT* BuildOnsetAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    static UConstantQNRT* BuildConstantQAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
};