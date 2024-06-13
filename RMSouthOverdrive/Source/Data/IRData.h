/*
==============================================================================

    IRData.h
    Author:  micheler1208

==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class IRData
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec spec);
    void process (juce::AudioBuffer<float>& buffer);
    void updateParameters ();
    void reset();
    void loadImpulseResponse();
    
private:    
    juce::dsp::Convolution convolutionProcessor;
    
    bool isPrepared { false };
};