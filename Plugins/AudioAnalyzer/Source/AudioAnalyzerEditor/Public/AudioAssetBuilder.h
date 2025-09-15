#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AudioAssetBuilder.generated.h"

// Forward Declerations
class USoundWave;
class ULoudnessNRT;
class UOnsetNRT;
class UConstantQNRT;

USTRUCT(BlueprintType)
struct FGeneratedNRTAssets
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ULoudnessNRT* LoudnessNRT = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UOnsetNRT* OnsetNRT = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UConstantQNRT* ConstantQNRT = nullptr;
};

UCLASS(BlueprintType, Blueprintable)
class UAudioAssetBuilder : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, CallInEditor, Category="AudioAnalyzerEditor")
    FGeneratedNRTAssets BuildAllAssets(USoundWave* SourceAudio, const FString& PackagePath);

    // Builders
    ULoudnessNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    // UOnsetNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
    // UConstantQNRT* BuildLoudnessAsset(USoundWave* SourceAudio, const FString& PackagePath, const FString& AssetName);
};