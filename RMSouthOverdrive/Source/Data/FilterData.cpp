/*
==============================================================================

    FilterData.cpp
    Author:  micheler1208

==============================================================================
*/

#include "FilterData.h"

// PREPARE TO PLAY
void FilterData::prepareToPlay (juce::dsp::ProcessSpec spec)
{
    reset();
    
    highPassFilter.prepare(spec);
    lowPassFilter.prepare(spec);
    
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(spec.sampleRate, 100.0f);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate, 11000.0f);
    
    isPrepared = true;
}

// PROCESS FILTER
void FilterData::process (juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    highPassFilter.process(context);
    lowPassFilter.process(context);
}

// UPDATE PARAMETERS
void FilterData::updateParameters ()
{
}

// RESET
void FilterData::reset()
{
    lowPassFilter.reset();
    highPassFilter.reset();
}