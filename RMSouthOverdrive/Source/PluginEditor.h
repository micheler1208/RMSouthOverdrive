/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "PluginProcessor.h"

class RM5150HarmonicEnhancerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                    private juce::Slider::Listener  
{
public:
    // CONSTRUCTOR AND DESTRUCTOR
    RM5150HarmonicEnhancerAudioProcessorEditor (RM5150HarmonicEnhancerAudioProcessor&);
    ~RM5150HarmonicEnhancerAudioProcessorEditor() override;

    
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void RM5150HarmonicEnhancerAudioProcessorEditor::drawLabelledSlider(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& name, juce::Slider& slider);

private:

    // PROCESSOR
    RM5150HarmonicEnhancerAudioProcessor& audioProcessor;
    
    // IMAGES
    juce::Image backgroundImage;
    
    // SLIDERS
    CustomLookAndFeel customLookAndFeel;
    juce::Slider driveSlider;
    juce::Slider bassSlider;
    juce::Slider midSlider;
    juce::Slider trebleSlider;
    juce::Slider outputVolumeSlider;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RM5150HarmonicEnhancerAudioProcessorEditor)
};
