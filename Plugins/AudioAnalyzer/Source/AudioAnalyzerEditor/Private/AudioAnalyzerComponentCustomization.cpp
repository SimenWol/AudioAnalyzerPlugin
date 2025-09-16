#include "AudioAnalyzerComponentCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "PropertyCustomizationHelpers.h"

#include "AudioAssetBuilder.h"

TSharedRef<IDetailCustomization> FAudioAnalyzerComponentCustomization::MakeInstance()
{
    return MakeShareable(new FAudioAnalyzerComponentCustomization);
}

void FAudioAnalyzerComponentCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    // Get selected object(s)
    TArray<TWeakObjectPtr<UObject>> Objects;
    DetailBuilder.GetObjectsBeingCustomized(Objects);

    if (Objects.Num() == 1)
    {
        CachedComponent = Cast<UAudioAnalyzerComponent>(Objects[0].Get());
    }

    // Find "SourceAudio" property
    TSharedPtr<IPropertyHandle> SourceAudioHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UAudioAnalyzerComponent, SourceAudio));

    if (SourceAudioHandle && SourceAudioHandle->IsValidHandle())
    {
        // Add callback
        SourceAudioHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FAudioAnalyzerComponentCustomization::OnSourceAudioChanged));
        // SourceAudioHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this, &FAudioAnalyzerComponentCustomization::OnSourceAudioChanged));
    }
}

void FAudioAnalyzerComponentCustomization::OnSourceAudioChanged()
{
    if (!CachedComponent.IsValid())
    {
        return;
    }

    USoundWave* Source = CachedComponent->SourceAudio;
    if (!Source)
    {
        return;
    }

    // Run builder
    UAudioAssetBuilder* Builder = NewObject<UAudioAssetBuilder>();
    FGeneratedNRTAssets Assets = Builder->BuildAllAssets(Source, TEXT("/Game/GeneratedNRT"));

    CachedComponent->ConstantQNRT = Assets.ConstantQNRT;
    CachedComponent->LoudnessNRT = Assets.LoudnessNRT;
    CachedComponent->OnsetNRT = Assets.OnsetNRT;

    CachedComponent->Modify();
}