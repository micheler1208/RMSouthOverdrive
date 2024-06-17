/*
==============================================================================

    EQData.cpp
    Author:  micheler1208

==============================================================================
*/

#include "EQData.h"
#include <algorithm> // Per std::max

// PREPARE TO PLAY
void EQData::prepareToPlay(juce::dsp::ProcessSpec spec)
{
    EQ.reset();
    sampleRate = spec.sampleRate;
    EQ.prepare(spec);
    isPrepared = true;

    updateValues(bass, mid, treble);
}

// PROCESS FILTER
void EQData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    EQ.process(context);
}

// UPDATE PARAMETERS
void EQData::updateValues(float bassValue, float midValue, float trebleValue)
{
    bass = bassValue;
    mid = midValue;
    treble = trebleValue;

    float adjustedBass = bass;
    float adjustedMid = mid * std::abs(mid) / 5.0f;
    float adjustedTreble = treble * std::abs(treble) / 5.0f;


    // Ensure gainFactor is always positive and non-zero
    updateFilter(0, std::max(adjustedBass, 0.0001f), 90.0f, 0.707f);   // BASS (Peak Filter)
    updateFilter(1, std::max(adjustedMid, 0.0001f), 500.0f, 0.707f);    // MID (Peak Filter)
    updateFilter(2, std::max(adjustedTreble, 0.0001f), 3000.0f, 0.707f); // TREBLE (Peak Filter)
}

// UPDATE FILTER
void EQData::updateFilter(int index, float gain, float frequency, float Q)
{
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, frequency, Q, std::pow(10.0f, gain / 20.0f));
    switch(index)
    {
        case 0:
            *EQ.get<0>().state = *coefficients;
            break;
        case 1:
            *EQ.get<1>().state = *coefficients;
            break;
        case 2:
            *EQ.get<2>().state = *coefficients;
            break;
    }
}

// RESET
void EQData::reset()
{
    EQ.reset();
}
