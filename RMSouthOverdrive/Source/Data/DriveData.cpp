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
void DriveData::process (juce::AudioBuffer<float>& buffer, int sample)
{
    jassert(isPrepared);
    
    auto* inputChannelData = buffer.getReadPointer(0);
    float cleanSignal = inputChannelData[sample];

    // First stage of distortion
    float gainStage1 = cleanSignal * drive;
    float clippedSignal1 = combinedClipping(gainStage1);

    // Smoothing between stages
    float smoothedSignal1 = smoothingFilter(clippedSignal1, prevSample, 0.1f);

    // Second stage of distortion
    float gainStage2 = smoothedSignal1 * drive;
    float clippedSignal2 = diodeClipping(gainStage2);

    // Smoothing between stages
    float smoothedSignal2 = smoothingFilter(clippedSignal2, prevSample, 0.1f);

    // Third stage of distortion
    float gainStage3 = smoothedSignal2 * drive;
    float clippedSignal3 = hardClipping(gainStage3);
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