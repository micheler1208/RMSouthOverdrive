/*
  ==============================================================================

    VolumeComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelDriveVolume.h"
#include "CustomSlider.h"

//==============================================================================
/*
*/
class VolumeComponent  : public juce::Component
{
public:
    VolumeComponent(juce::AudioProcessorValueTreeState& apvts);
    ~VolumeComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // CUSTOM LOOK AND FEEL
    CustomLookAndFeelDriveVolume customLookAndFeelDriveVolume;
    
    // VOLUME
    CustomSlider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    // LABELS
    juce::Label volumeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeComponent)
};
