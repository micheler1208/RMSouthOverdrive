/*
  ==============================================================================

    AdsrComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelYellow.h"
#include "CustomSlider.h"

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // ADSR
    CustomSlider attackSlider, decaySlider, sustainSlider, releaseSlider;

    // LABELS
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    // ADSR ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    // SLIDERS CUSTOM STYLE
    CustomLookAndFeelYellow customLookAndFeelYellow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
