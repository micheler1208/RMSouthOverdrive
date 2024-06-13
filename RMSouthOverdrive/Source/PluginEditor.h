/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "UI/EQComponent.h"
#include "UI/VolumeComponent.h"
#include "UI/DriveComponent.h"
#include "PluginProcessor.h"

class RMSouthOverdriveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    RMSouthOverdriveAudioProcessorEditor (RMSouthOverdriveAudioProcessor&);
    ~RMSouthOverdriveAudioProcessorEditor() override;

    
    void paint (juce::Graphics&) override;
    void resized() override;
    void RMSouthOverdriveAudioProcessorEditor::drawLabelledSlider(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& name, juce::Slider& slider);

private:

    // PROCESSOR
    RMSouthOverdriveAudioProcessor& audioProcessor;
    

    //DRIVE
    DriveComponent drive;
    
    //EQ
    EQComponent EQ;
    
    //VOLUME
    VolumeComponent volume;
    
    // IMAGES
    juce::Image backgroundImage;
    
    //LOAD IR BUTTON
    juce::TextButton loadIRButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMSouthOverdriveAudioProcessorEditor)
};
