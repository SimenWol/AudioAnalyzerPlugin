#include "AudioAnalyzerDebugWidget.h"
#include "AudioAnalyzerManager.h"
#include "AudioAnalyzerDebugProxy.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Log.h"

void SAudioAnalyzerDebugWidget::Construct(const FArguments& InArgs)
{
    // Create the delegate proxy UObject
    DelegateProxy = NewObject<UAudioAnalyzerDebugProxy>();
    DelegateProxy->SetWidget(SharedThis(this));

    // Add to root so it doesn't get garbage collected
    DelegateProxy->AddToRoot();
    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("DelegateProxy created and set."));

    // Construct widget
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
            .Padding(0, 10, 0, 2)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Loudness:"))
                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 2, 0, 2)
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
                .Padding(2.0f)
                [
                    SNew(SProgressBar)
                    .Percent(this, &SAudioAnalyzerDebugWidget::GetLoudnessPercent)
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 0, 0, 5)
            [
                SNew(STextBlock)
                .Text(this, &SAudioAnalyzerDebugWidget::GetLoudnessText)
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
            ]

            // Beat Indicator
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0, 5)
            [
                SNew(SBorder)
                .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
                .BorderBackgroundColor(this, &SAudioAnalyzerDebugWidget::GetBeatIndicatorColor)
                .Padding(20.0f)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("BEAT INDICATOR"))
                    .Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
                    .Justification(ETextJustify::Center)
                ]
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

SAudioAnalyzerDebugWidget::~SAudioAnalyzerDebugWidget()
{
    // Clean up delegates
    if (AnalyzerManager.IsValid() && DelegateProxy)
    {
        AnalyzerManager->OnLoudnessChanged.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnLoudnessChanged);
        AnalyzerManager->OnsetDetected.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnOnsetDetected);
        AnalyzerManager->OnConstantQChanged.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnConstantQChanged);
        AnalyzerManager->OnBeatDetected.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnBeatDetected);
    }

    if (DelegateProxy)
    {
        DelegateProxy->RemoveFromRoot();
    }

    UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("AudioAnalyzerDebugWidget destroyed."));
}

void SAudioAnalyzerDebugWidget::SetAnalyzerManager(UAudioAnalyzerManager* InManager)
{    
    // Unbind previous delegates
    if (AnalyzerManager.IsValid() && DelegateProxy)
    {
        UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("Unbinding previous manager delegates."));
        AnalyzerManager->OnLoudnessChanged.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnLoudnessChanged);
        AnalyzerManager->OnsetDetected.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnOnsetDetected);
        AnalyzerManager->OnConstantQChanged.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnConstantQChanged);
        AnalyzerManager->OnBeatDetected.RemoveDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnBeatDetected);
    }
    
    AnalyzerManager = InManager;
    
    // Bind new delegates
    if (AnalyzerManager.IsValid() && DelegateProxy)
    {
        UE_LOG(LogAudioAnalyzerEditor, Log, TEXT("Binding new manager delegates."));
        AnalyzerManager->OnLoudnessChanged.AddDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnLoudnessChanged);
        AnalyzerManager->OnsetDetected.AddDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnOnsetDetected);
        AnalyzerManager->OnConstantQChanged.AddDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnConstantQChanged);
        AnalyzerManager->OnBeatDetected.AddDynamic(DelegateProxy, &UAudioAnalyzerDebugProxy::OnBeatDetected);
    }
    else
    {
        UE_LOG(LogAudioAnalyzerEditor, Warning, TEXT("Binding failed - Manager or Proxy is null."));
    }
}

void SAudioAnalyzerDebugWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    // Decay beat flash intensity over time
    if (BeatFlashIntensity > 0.0f)
    {
        BeatFlashIntensity = FMath::Max(0.0f, BeatFlashIntensity - (InDeltaTime * BeatFlashDecayRate));
    }
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
    // Flash beat indicator
    BeatFlashIntensity = 1.0f;

    // Add to log
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
    return FText::FromString(FString::Printf(TEXT("%.2f dB (%.1f%%)"), CurrentLoudness, CurrentLoudness * 100.0f));
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

FSlateColor SAudioAnalyzerDebugWidget::GetBeatIndicatorColor() const
{
    // Interpolate between neutral and beat colors based on flash intensity
    // Change color values below if wanted
    const FLinearColor NeutralColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
    const FLinearColor BeatColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

    return FSlateColor(FLinearColor::LerpUsingHSV(NeutralColor, BeatColor, BeatFlashIntensity));
}

TOptional<float> SAudioAnalyzerDebugWidget::GetLoudnessPercent() const
{
    // Clamp between 0 and 1 just to be safe, even though input should already be normalized - TODO: preferable send non-normalized dB values to debug widget
    return TOptional<float>(FMath::Clamp(CurrentLoudness, 0.0f, 1.0f));
}