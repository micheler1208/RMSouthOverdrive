/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "UI/CustomLookAndFeel.h"
#include "PluginProcessor.h"

class RMSouthOverdriveAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                    private juce::Slider::Listener  
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    RMSouthOverdriveAudioProcessorEditor (RMSouthOverdriveAudioProcessor&);
    ~RMSouthOverdriveAudioProcessorEditor() override;

    
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void RMSouthOverdriveAudioProcessorEditor::drawLabelledSlider(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& name, juce::Slider& slider);

private:

    // PROCESSOR
    RMSouthOverdriveAudioProcessor& audioProcessor;
    
    // IMAGES
    juce::Image backgroundImage;
    
    // SLIDERS
    CustomLookAndFeel customLookAndFeel;
    juce::Slider driveSlider;
    juce::Slider bassSlider;
    juce::Slider midSlider;
    juce::Slider trebleSlider;
    juce::Slider outputVolumeSlider;
    
    //LOAD IR BUTTON
    juce::TextButton loadIRButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMSouthOverdriveAudioProcessorEditor)
};
