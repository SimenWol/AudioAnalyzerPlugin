#pragma once

#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class UAudioAnalyzerComponent;

class FAudioAnalyzerComponentCustomization : public IDetailCustomization
{
public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    // Called when customizing the details of UAudioAnalyzerComponent
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
    void OnSourceAudioChanged();

    TWeakObjectPtr<UAudioAnalyzerComponent> CachedComponent;
}