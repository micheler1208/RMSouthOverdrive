/*
  ==============================================================================

    EQComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelEQ.h"
#include "CustomSlider.h"

//==============================================================================
/*
*/
class EQComponent  : public juce::Component
{
public:
    EQComponent(juce::AudioProcessorValueTreeState& apvts);
    ~EQComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:


    // CUSTOM LOOK AND FEEL
    CustomLookAndFeelEQ customLookAndFeelEQ;

    // SLIDERS
    CustomSlider bassSlider, midSlider, trebleSlider;

    // LABELS
    juce::Label bassLabel, midLabel, trebleLabel;

    // EQ ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQComponent)
};
