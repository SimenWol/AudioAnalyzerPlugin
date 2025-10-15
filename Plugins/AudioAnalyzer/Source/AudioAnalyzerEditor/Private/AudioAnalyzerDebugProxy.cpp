#include "AudioAnalyzerDebugProxy.h"
#include "AudioAnalyzerDebugWidget.h"

void UAudioAnalyzerDebugProxy::SetWidget(TSharedPtr<SAudioAnalyzerDebugWidget> InWidget)
{
    Widget = InWidget;
}

void UAudioAnalyzerDebugProxy::OnLoudnessChanged(float Loudness)
{
    if (TSharedPtr<SAudioAnalyzerDebugWidget> PinnedWidget = Widget.Pin())
    {
        PinnedWidget->OnLoudnessChanged(Loudness);
    }
}

void UAudioAnalyzerDebugProxy::OnOnsetDetected(float TimeSeconds, float Strength)
{
    if (TSharedPtr<SAudioAnalyzerDebugWidget> PinnedWidget = Widget.Pin())
    {
        PinnedWidget->OnOnsetDetected(TimeSeconds, Strength);
    }
}

void UAudioAnalyzerDebugProxy::OnConstantQChanged(const TArray<float>& Values, int32 ChannelIndex)
{
    if (TSharedPtr<SAudioAnalyzerDebugWidget> PinnedWidget = Widget.Pin())
    {
        PinnedWidget->OnConstantQChanged(Values, ChannelIndex);
    }
}

void UAudioAnalyzerDebugProxy::OnBeatDetected(float TimeSeconds)
{
    if (TSharedPtr<SAudioAnalyzerDebugWidget> PinnedWidget = Widget.Pin())
    {
        PinnedWidget->OnBeatDetected(TimeSeconds);
    }
}