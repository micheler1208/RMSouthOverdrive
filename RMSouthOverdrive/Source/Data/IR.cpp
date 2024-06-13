/*
==============================================================================

    IR.cpp
    Author:  micheler1208

==============================================================================
*/

#include "IR.h"

// PREPARE TO PLAY
void IR::prepareToPlay ()
{
    IR.reset();   
    isPrepared = true;
}

// PROCESS FILTER
void IR::process (juce::AudioBuffer<float>& buffer)
{
    jassert (isPrepared);
    
    // Process IR convolution
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    convolutionProcessor.process(context);
}


//LOAD IMPULSE RESPONSE
void IR::loadImpulseResponse()
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
void IR::updateParameters ()
{
}

// RESET
void IR::reset()
{
    convolutionProcessor.reset();
}