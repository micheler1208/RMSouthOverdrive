/*
  ==============================================================================

    PluginProcessor.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Data/DriveData.h"
#include "Data/EQData.h"
#include "Data/VolumeData.h"
#include "Data/FilterData.h"
#include "Data/IRData.h"
#include "Data/EQData.h"

class RMSouthOverdriveAudioProcessor : public juce::AudioProcessor
{
public:
    RMSouthOverdriveAudioProcessor();
    ~RMSouthOverdriveAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;


    IRData& getIR() { return IR; };
    
    juce::AudioProcessorValueTreeState apvts;

    
private:
    //Create Params
    juce::AudioProcessorValueTreeState::ParameterLayout createParams(); 
    
    //Drive
    DriveData drive;
    
    //Volume
    VolumeData volume;

    //Filters LP,HP
    FilterData filters;

    //IR
    IRData IR;
    //EQ
    EQData EQ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RMSouthOverdriveAudioProcessor)
};
