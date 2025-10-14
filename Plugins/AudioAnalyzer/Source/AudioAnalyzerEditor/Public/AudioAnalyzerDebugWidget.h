#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UAudioAnalyzerManager;

struct FDebugEventEntry
{
    FString EventType;
    FString Details;
    double Timestamp;
    FLinearColor Color;
};

class SAudioAnalyzerDebugWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAudioAnalyzerDebugWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    void SetAnalyzerManager(UAudioAnalyzerManager* InManager);

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    TWeakObjectPtr<UAudioAnalyzerManager> AnalyzerManager;
    TArray<FDebugEventEntry> EventLog;
    int32 MaxLogEntries = 100;

    // Current values
    float CurrentLoudness = 0.0f;
    TArray<float> CurrentConstantQ;

    // Event callbacks
    void OnLoudnessChanged(float Loudness);
    void OnOnsetDetected(float TimeSeconds, float Strength);
    void OnConstantQChanged(const TArray<float>& Values, int32 ChannelIndex);
    void OnBeatDetected(float TimeSeconds);

    void AddEventToLog(const FString& EventType, const FString& Details, const FLinearColor& Color);

    FText GetLoudnessText() const;
    FText GetEventLogText() const;
};