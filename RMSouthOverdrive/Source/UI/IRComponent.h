/*
  ==============================================================================

    IRComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h" 
#include "CustomLookAndFeelIR.h" 

//==============================================================================
/*
*/
class IRComponent  : public juce::Component
{
public:
    IRComponent(juce::AudioProcessorValueTreeState& apvts, RMSouthOverdriveAudioProcessor& processor);
    ~IRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RMSouthOverdriveAudioProcessor& audioProcessor;
    CustomLookAndFeelIR customLookAndFeelIR;
    juce::TextButton loadIRButton;
    juce::Label curvedLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRComponent)
};
