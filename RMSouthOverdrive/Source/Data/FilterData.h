/*
==============================================================================

    FilterData.h
    Author:  micheler1208

==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class FilterData
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec spec);
    void process (juce::AudioBuffer<float>& buffer);
    void updateParameters ();
    void reset();
    
private:    
    // DSP filters
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    using FilterDuplicator = juce::dsp::ProcessorDuplicator<Filter, Coefficients>;
    FilterDuplicator highPassFilter;
    FilterDuplicator lowPassFilter;
    
    bool isPrepared { false };
};