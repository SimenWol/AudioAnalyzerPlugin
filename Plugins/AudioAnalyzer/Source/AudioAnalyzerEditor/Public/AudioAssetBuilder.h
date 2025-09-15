#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAssetBuilder.generated.h"

// Forward Declerations
class USoundWave;
class ULoudnessNRT;
class UOnsetNRT;
class UConstantQNRT;

UCLASS(BlueprintType, Blueprintable)
class UAudioAssetBuilder : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, CallInEditor, Category="AudioAnalyzerEditor")
    void BuildAllAssets(USoundWave* SourceAudio, const FString& PackagePath);

    // Builders
    ULoudnessNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    // UOnsetNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    // UConstantQNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
};