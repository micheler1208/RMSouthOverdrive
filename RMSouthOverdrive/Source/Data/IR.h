/*
==============================================================================

    IR.h
    Author:  micheler1208

==============================================================================
*/

#pragma once


class IR
{
public:
    void prepareToPlay ();
    void process (juce::AudioBuffer<float>& buffer);
    void updateParameters ();
    void reset();
    void loadImpulseResponse();
    
private:    
    juce::dsp::Convolution convolutionProcessor;
    
    bool isPrepared { false };
};