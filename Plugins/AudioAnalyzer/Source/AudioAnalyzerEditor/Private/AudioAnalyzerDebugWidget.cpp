#include "AudioAnalyzerDebugWidget.h"
#include "AudioAnalyzerManager.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"

void SAudioAnalyzerDebugWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
        .Padding(10.0f)
        [
            SNew(SVerticalBox)

            // Title
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 10)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Audio Analyzer Debug"))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
            ]

            // Current Loudness
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 5)
            [
                SNew(STextBlock)
                .Text(this, &SAudioAnalyzerDebugWidget::GetLoudnessText)
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
            ]

            // Event Log Header
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 10, 0, 5)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Event Log:"))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
            ]

            // Event Log (scrollable)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(SScrollBox)
                + SScrollBox::Slot()
                [
                    SNew(STextBlock)
                    .Text(this, &SAudioAnalyzerDebugWidget::GetEventLogText)
                    .Font(FCoreStyle::GetDefaultFontStyle("Mono", 10))
                    .AutoWrapText(false)
                ]
            ]
        ]
    ];
}

void SAudioAnalyzerDebugWidget::SetAnalyzerManager(UAudioAnalyzerManager* InManager)
{
    // TODO: look into how to do this because it does not support casting from S to U
    
    // Unbind previous delegates
    if (AnalyzerManager.IsValid())
    {
        // AnalyzerManager->OnLoudnessChanged.RemoveDynamic(this, &SAudioAnalyzerDebugWidget::OnLoudnessChanged);
        // AnalyzerManager->OnsetDetected.RemoveDynamic(this, &SAudioAnalyzerDebugWidget::OnOnsetDetected);
        // AnalyzerManager->OnConstantQChanged.RemoveDynamic(this, &SAudioAnalyzerDebugWidget::OnConstantQChanged);
        // AnalyzerManager->OnBeatDetected.RemoveDynamic(this, &SAudioAnalyzerDebugWidget::OnBeatDetected);
    }
    
    AnalyzerManager = InManager;
    
    // Bind new delegates
    if (AnalyzerManager.IsValid())
    {
        // AudioComp->OnAudioPlaybackPercent.AddDynamic(this, &UAudioAnalyzerComponent::OnPlaybackPercentChanged);

        // AnalyzerManager->OnLoudnessChanged.AddDynamic(this, &SAudioAnalyzerDebugWidget::OnLoudnessChanged);
        // AnalyzerManager->OnsetDetected.AddDynamic(this, &SAudioAnalyzerDebugWidget::OnOnsetDetected);
        // AnalyzerManager->OnConstantQChanged.AddDynamic(this, &SAudioAnalyzerDebugWidget::OnConstantQChanged);
        // AnalyzerManager->OnBeatDetected.AddDynamic(this, &SAudioAnalyzerDebugWidget::OnBeatDetected);
    }
}

void SAudioAnalyzerDebugWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SAudioAnalyzerDebugWidget::OnLoudnessChanged(float Loudness)
{
    CurrentLoudness = Loudness;
    AddEventToLog("Loudness", FString::Printf(TEXT("%.2f dB"), Loudness), FLinearColor::Green);
}

void SAudioAnalyzerDebugWidget::OnOnsetDetected(float TimeSeconds, float Strength)
{
    AddEventToLog("Onset", FString::Printf(TEXT("Time: %.2fs, Strength: %.2f"), TimeSeconds, Strength), FLinearColor::Yellow);
}

void SAudioAnalyzerDebugWidget::OnConstantQChanged(const TArray<float>& Values, int32 ChannelIndex)
{
    CurrentConstantQ = Values;
    AddEventToLog("ConstantQ", FString::Printf(TEXT("Channel %d updated"), ChannelIndex), FLinearColor::Blue);
}

void SAudioAnalyzerDebugWidget::OnBeatDetected(float TimeSeconds)
{
    AddEventToLog("BEAT", FString::Printf(TEXT("Time: %.2fs"), TimeSeconds), FLinearColor::Red);
}

void SAudioAnalyzerDebugWidget::AddEventToLog(const FString& EventType, const FString& Details, const FLinearColor& Color)
{
    FDebugEventEntry Entry;
    Entry.EventType = EventType;
    Entry.Details = Details;
    Entry.Timestamp = FPlatformTime::Seconds();
    Entry.Color = Color;

    EventLog.Insert(Entry, 0);

    if (EventLog.Num() > MaxLogEntries)
    {
        EventLog.SetNum(MaxLogEntries);
    }
}

FText SAudioAnalyzerDebugWidget::GetLoudnessText() const
{
    return FText::FromString(FString::Printf(TEXT("Current Loudness: %.2f dB"), CurrentLoudness));
}

FText SAudioAnalyzerDebugWidget::GetEventLogText() const
{
    FString LogText;
    for (const FDebugEventEntry& Entry : EventLog)
    {
        LogText += FString::Printf(TEXT("[%.2f] %s: %s\n"),
            Entry.Timestamp,
            *Entry.EventType,
            *Entry.Details);
    }
    
    return FText::FromString(LogText);
}