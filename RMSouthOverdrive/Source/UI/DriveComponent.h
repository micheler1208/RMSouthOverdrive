/*
  ==============================================================================

    DriveComponent.h
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
class DriveComponent  : public juce::Component
{
public:
    DriveComponent(juce::AudioProcessorValueTreeState& apvts);
    ~DriveComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // CUSTOM LOOK AND FEEL
    CustomLookAndFeelDriveVolume customLookAndFeelDriveVolume;
    
    // DRIVE
    CustomSlider driveSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    // LABELS
    juce::Label driveLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DriveComponent)
};
