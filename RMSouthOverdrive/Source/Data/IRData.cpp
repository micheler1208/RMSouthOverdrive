/*
==============================================================================

    IRData.cpp
    Author:  micheler1208

==============================================================================
*/

#include "IRData.h"

// PREPARE TO PLAY
void IRData::prepareToPlay (juce::dsp::ProcessSpec spec)
{
    reset(); 
    convolutionProcessor.prepare(spec);
    isPrepared = true;
}

// PROCESS FILTER
void IRData::process (juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    
    // Process IR convolution
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    convolutionProcessor.process(context);
}


//LOAD IMPULSE RESPONSE
void IRData::loadImpulseResponse()
{
    DBG("Opening File Chooser...");
    juce::FileChooser chooser("Select an Impulse Response File", {}, "*.wav");
    if (chooser.browseForFileToOpen())
    {
        juce::File irFile = chooser.getResult();
        DBG("File selected: " << irFile.getFullPathName());
        if (irFile.existsAsFile())
        {
            convolutionProcessor.loadImpulseResponse(irFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0, juce::dsp::Convolution::Normalise::yes);
            DBG("Impulse response loaded.");
        }
    }
    else
    {
        DBG("No file selected.");
    }
}


// UPDATE PARAMETERS
void IRData::updateParameters ()
{
}

// RESET
void IRData::reset()
{
    convolutionProcessor.reset();
}