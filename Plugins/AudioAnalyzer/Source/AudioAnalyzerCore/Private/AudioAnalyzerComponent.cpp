#include "AudioAnalyzerComponent.h"
#include "AudioAnalyzerManager.h"

#include "Log.h"

UAudioAnalyzerComponent::UAudioAnalyzerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    // TimeElapsed = 0.0f;
}

void UAudioAnalyzerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (SourceAudio)
    {
        if (!AnalyzerManager)
        {
            AnalyzerManager = NewObject<UAudioAnalyzerManager>(this);
        }
        
        AnalyzerManager->InitializeAssets(LoudnessNRT, OnsetNRT, ConstantQNRT);

        // if (LoudnessNRT != nullptr) {UE_LOG(LogAudioAnalyzerCore, Log, TEXT("%f"), LoudnessNRT->Settings->AnalysisPeriod);}
        // else UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("NO LOUDNESSNRT :()"));

        // // check for first 5 seconds
        // FOnsetData Onsets = AnalyzerManager->GetOnSetsBetweenTimes(0.0f, 5.0f, 0);

        // for (int32 i = 0; i < Onsets.Timestamps.Num(); ++i)
        // {
        //     UE_LOG(LogTemp, Log, TEXT("Onset at %f sec, strength %f"), Onsets.Timestamps[i], Onsets.Strengths[i]);
        // }
    }
}

// temporary test function
void UAudioAnalyzerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Possibly more implementation later - mainly used for testing currently

    // TimeElapsed += DeltaTime;

    // if (AnalyzerManager)
    // {
    //     float Loudness = AnalyzerManager->GetLoudnessAtTime(TimeElapsed);
    //     UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Loudness at %.2f = %.3f"), TimeElapsed, Loudness);

    //     TArray<float> Spectrum = AnalyzerManager->GetConstantQAtTime(TimeElapsed, 0);
    //     UE_LOG(LogAudioAnalyzerCore, Log, TEXT("ConstantQSpectrum bins: %d"), Spectrum.Num());

    //     //
    // }
}


// temporary TODO: move to editor module
#if WITH_EDITOR

#include "AudioAssetBuilder.h"

void UAudioAnalyzerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, SourceAudio))
    {
        if (SourceAudio)
        {
            // Build new NRT assets
            FString PackagePath = TEXT("/Game/GeneratedAudioAnalysis"); // you can make this configurable
            FGeneratedNRTAssets NewAssets = UAudioAssetBuilder::BuildAllAssets(SourceAudio, PackagePath);

            if (NewAssets.LoudnessNRT == nullptr) { UE_LOG(LogAudioAnalyzerCore, Warning, TEXT("BuildAllAssets returned nullptr for LoudnessNRT")); }

            // Attaching assets on component still broken. TODO: revisit & fix.
            if (NewAssets.LoudnessNRT)
            {
                Modify(); // record transaction on *this* component
                LoudnessNRT   = NewAssets.LoudnessNRT;
                OnsetNRT      = NewAssets.OnsetNRT;
                ConstantQNRT  = NewAssets.ConstantQNRT;

                if (UObject* Outer = GetOuter())
                {
                    Outer->MarkPackageDirty();
                }

                // Trigger editor refresh of the property field
                FProperty* ChangedProp = FindFProperty<FProperty>(UAudioAnalyzerComponent::StaticClass(),
                    GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, LoudnessNRT));

                if (ChangedProp)
                {
                    FPropertyChangedEvent ChangedEvent(ChangedProp);
                    PostEditChangeProperty(ChangedEvent);
                }

                UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Assigned LoudnessNRT %s"), *LoudnessNRT->GetPathName());

                if (LoudnessNRT)
                {
                    UE_LOG(LogAudioAnalyzerCore, Log, TEXT("Final Assigned LoudnessNRT Path: %s"), *LoudnessNRT->GetPathName());
                }
            }
        }
        else
        {
            // If audio was cleared, clear analyzers too
            LoudnessNRT  = nullptr;
            OnsetNRT     = nullptr;
            ConstantQNRT = nullptr;

            if (AActor* Owner = GetOwner())
            {
                Owner->Modify();
                Owner->MarkPackageDirty();
            }
        }
    }
}
#endif