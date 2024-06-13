/*
==============================================================================

    EQData.h
    Author:  micheler1208

==============================================================================
*/

#pragma once


class EQData
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec spec);
    void process (juce::AudioBuffer<float>& buffer);
    void updateValues(float bassValue, float midValue, float trebleValue);
    void reset();
    
private:    
    float bass, mid, treble;
    juce::dsp::ProcessorChain<FilterDuplicator, FilterDuplicator, FilterDuplicator> EQ;
    bool isPrepared { false };
};