/*
  ==============================================================================

    DriveData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DriveData
{
public:
    void prepareToPlay();
    void updateValue(const float driveValue);
    void reset();
    void process(juce::AudioBuffer<float>& buffer);
    
private:
    float drive;
    std::vector<float> prevSamples;
    bool isPrepared { false };
    
    //Clipping functions
    float softClipping(float x);
    float hardClipping(float x);
    float combinedClipping(float x);
    float diodeClipping(float x);
    
    //Saturation
    float saturation(float x);
    
    //Smoothing Filter
    float smoothingFilter(float currentSample, float& prevSample, float smoothingFactor);
};