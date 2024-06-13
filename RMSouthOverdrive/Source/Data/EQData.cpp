/*
==============================================================================

    EQData.cpp
    Author:  micheler1208

==============================================================================
*/

#include "EQData.h"

// PREPARE TO PLAY
void EQData::prepareToPlay (juce::dsp::ProcessSpec spec)
{
    EQ.reset();
    
    auto sampleRate = spec.sampleRate;
    
    bass = 0.0f;
    mid = 0.0f;
    treble = 0.0f;

    // Apply a power function to make the knobs less sensitive
    float adjustedBass = bass * std::abs(bass) / 5.0f;
    float adjustedMid = mid * std::abs(mid) / 5.0f;
    float adjustedTreble = treble * std::abs(treble) / 5.0f;

    // BASS (Peak Filter)
    float bassGainDB = adjustedBass;  // ±5 dB range
    auto bassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 100.0f, 0.707f, std::pow(10.0f, bassGainDB / 20.0f));
    EQ.get<0>().state = *bassCoefficients;

    // MID (Peak Filter)
    float midGainDB = adjustedMid;  // ±5 dB range
    auto midCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 650.0f, 0.707f, std::pow(10.0f, midGainDB / 20.0f));
    EQ.get<1>().state = *midCoefficients;

    // TREBLE (Peak Filter)
    float trebleGainDB = adjustedTreble;  // ±5 dB range
    auto trebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 2800.0f, 0.707f, std::pow(10.0f, trebleGainDB / 20.0f));
    EQ.get<2>().state = *trebleCoefficients;
    
    EQ.prepare(spec);
    
    isPrepared = true;
}

// PROCESS FILTER
void EQData::process (juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    EQ.process(context);
}

// UPDATE PARAMETERS
void EQData::updateValues (float bassValue, float midValue, float trebleValue)
{
    bass = bassValue;
    mid = midValue;
    treble = trebleValue;
}

// RESET
void EQData::reset()
{
    EQ.reset();
}