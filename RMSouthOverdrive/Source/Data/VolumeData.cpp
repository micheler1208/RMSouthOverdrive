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
    volume.reset();
    volume = 0.75f;
    
    isPrepared = true;
}

// PROCESS FILTER
void VolumeData::process (juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    
    auto* inputChannelData = buffer.getReadPointer(0);
    float distortedSignal = inputChannelData[sample];
    
    float adjustedOutputVolume = std::pow(*volume, 5.0f);
    float processedSignal = distortedSignal * adjustedOutputVolume;
}

// UPDATE PARAMETERS
void VolumeData::updateValue (const float volumeValue)
{
    volume = volumeValue;
}

// RESET
void VolumeData::reset()
{
    volume.reset();
}