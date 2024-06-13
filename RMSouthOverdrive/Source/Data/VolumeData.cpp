/*
==============================================================================

    VolumeData.cpp
    Author:  micheler1208

==============================================================================
*/

#include "VolumeData.h"

// PREPARE TO PLAY
void VolumeData::prepareToPlay()
{
    reset();
    volume = 0.75f;
    
    isPrepared = true;
}

// PROCESS BUFFER
void VolumeData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    auto totalNumInputChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float distortedSignal = channelData[sample];
            float adjustedOutputVolume = std::pow(volume, 5.0f);
            float processedSignal = distortedSignal * adjustedOutputVolume;

            // Store the processed sample back into the buffer
            channelData[sample] = processedSignal;
        }
    }
}

// UPDATE PARAMETERS
void VolumeData::updateValue (const float volumeValue)
{
    volume = volumeValue;
}

// RESET
void VolumeData::reset()
{
    volume = 0.75f;
}