/*
  ==============================================================================

    DriveData.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "DriveData.h"

// PREPARE TO PLAY
void DriveData::prepareToPlay()
{
    reset();
    drive = 5.0f;
    isPrepared = true;
}

// PROCESS FILTER
void DriveData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    auto totalNumInputChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    // Ensure prevSamples has the correct size
    if (prevSamples.size() != totalNumInputChannels)
    {
        prevSamples.resize(totalNumInputChannels, 0.0f);
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float cleanSignal = channelData[sample];

            // First stage of distortion
            float gainStage1 = cleanSignal * drive;
            float clippedSignal1 = combinedClipping(gainStage1);

            // Smoothing between stages
            float smoothedSignal1 = smoothingFilter(clippedSignal1, prevSamples[channel], 0.1f);

            // Second stage of distortion
            float gainStage2 = smoothedSignal1 * drive;
            float clippedSignal2 = diodeClipping(gainStage2);

            // Smoothing between stages
            float smoothedSignal2 = smoothingFilter(clippedSignal2, prevSamples[channel], 0.1f);

            // Third stage of distortion
            float gainStage3 = smoothedSignal2 * drive;
            float clippedSignal3 = hardClipping(gainStage3);

            // Store the processed sample back into the buffer
            channelData[sample] = clippedSignal3;

            // Update prevSample for the next iteration
            prevSamples[channel] = smoothedSignal2;
        }
    }
}

// UPDATE PARAMETERS
void DriveData:: updateValue (const float driveValue)
{
    drive = driveValue;
}

// RESET
void DriveData::reset()
{
    drive = 5.0f;
    prevSamples.clear();
}

// Soft Clipping Function
float DriveData::softClipping(float x)
{
    const float a = 2.0f;
    const float b = 1.0f;
    return (a * x) / (1 + b * std::abs(x));
}

// Hard Clipping Function
float DriveData::hardClipping(float x)
{
    const float positiveThreshold = 0.8f;
    const float negativeThreshold = -0.8f;
    if (x > positiveThreshold) return positiveThreshold;
    else if (x < negativeThreshold) return negativeThreshold;
    else return x;
}

// Combined Clipping
float DriveData::combinedClipping(float x) {
    float softClipped = softClipping(x);
    return hardClipping(softClipped);
}

// Diode Clipping
float DriveData::diodeClipping(float x) {
    const float threshold = 0.6f; 
    if (x > threshold) return threshold;
    else if (x < -threshold) return -threshold;
    else return x - (std::pow(x, 3) / 3);
}

// Saturation Function
float DriveData::saturation(float x)
{
    return std::atan(x);
}

// Smoothing Filter
float DriveData::smoothingFilter(float currentSample, float& prevSample, float smoothingFactor)
{
    float smoothedSample = smoothingFactor * prevSample + (1.0f - smoothingFactor) * currentSample;
    prevSample = smoothedSample;
    return smoothedSample;
}