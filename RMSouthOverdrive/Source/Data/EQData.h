/*
==============================================================================

    EQData.h
    Author:  micheler1208

==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class EQData
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec spec);
    void process (juce::AudioBuffer<float>& buffer);
    void updateValues(float bassValue, float midValue, float trebleValue);
    void reset();
    
private:    
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    using FilterDuplicator = juce::dsp::ProcessorDuplicator<Filter, Coefficients>;

    float bass, mid, treble;

    double sampleRate;

    juce::dsp::ProcessorChain<FilterDuplicator, FilterDuplicator, FilterDuplicator> EQ;

    void updateFilter(int index, float gain, float frequency, float Q);
    bool isPrepared { false };
};