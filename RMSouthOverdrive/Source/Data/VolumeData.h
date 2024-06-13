/*
==============================================================================

    VolumeData.h
    Author:  micheler1208

==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class VolumeData
{
public:
    void prepareToPlay ();
    void process (const float distorteSignal);
    void updateValue (const float volumeValue);
    void reset();
    
private:
    float volume;
    bool isPrepared { false };
};