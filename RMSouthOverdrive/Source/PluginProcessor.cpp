/*
  ==============================================================================

    PluginProcessor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

// CONSTRUCTOR
RMSouthOverdriveAudioProcessor::RMSouthOverdriveAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput("Input", juce::AudioChannelSet::mono(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "PARAMETERS", createParams())
{
}


// DESTRUCTOR
RMSouthOverdriveAudioProcessor::~RMSouthOverdriveAudioProcessor()
{    
}


// INITIALIZATION CONFIG
void RMSouthOverdriveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = getTotalNumOutputChannels();

    drive.prepareToPlay();
    volume.prepareToPlay();
    filters.prepareToPlay(spec);
    IR.prepareToPlay();
    EQ.prepareToPlay(spec);
}


// PAUSE/STOP CONFIG
void RMSouthOverdriveAudioProcessor::releaseResources()
{
}


// NEW PROCESS BLOCK
void RMSouthOverdriveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    
    //INIZIALIZE CHANNELS AND SAMPLEz
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    //CLEAR BUFFER
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    //GAIN STAGES
    if (totalNumInputChannels > 0 && totalNumOutputChannels > 1) {
        
        auto* leftOutputChannelData = buffer.getWritePointer(0);
        auto* rightOutputChannelData = buffer.getWritePointer(1);

        for (int sample = 0; sample < numSamples; ++sample) {
            
            float driveValue = apvts.getRawParameterValue("DRIVE")->load();
            drive.updateValue(driveValue);
            drive.process(buffer, sample);
                       
            float volumeValue = apvts.getRawParameterValue("VOLUME")->load();
            volume.updateValue(volumeValue);
            volume.process(buffer, sample);
            
            auto* processedData = buffer.getReadPointer(0);

            leftOutputChannelData[sample] = processedData[sample];
            rightOutputChannelData[sample] = processedData[sample];
        }

        filters.process(buffer);
        
        IR.process(buffer);
        
        float bassValue = apvts.getRawParameterValue("BASS")->load();
        float midValue = apvts.getRawParameterValue("MID")->load();
        float trebleValue = apvts.getRawParameterValue("TREBLE")->load();
        EQ.updateValues(bassValue, midValue, trebleValue);
        EQ.process(buffer);
    }
}


// GET NAME
const juce::String RMSouthOverdriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


// MIDI CONFIG
bool RMSouthOverdriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}


bool RMSouthOverdriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}


bool RMSouthOverdriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}


// TAIL AUDIO CONFIG
double RMSouthOverdriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}


// PROGRAM CONFIG
int RMSouthOverdriveAudioProcessor::getNumPrograms()
{
    return 1;
}


int RMSouthOverdriveAudioProcessor::getCurrentProgram()
{
    return 0;
}


void RMSouthOverdriveAudioProcessor::setCurrentProgram (int index)
{
}


const juce::String RMSouthOverdriveAudioProcessor::getProgramName (int index)
{
    return {};
}


void RMSouthOverdriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}


// OUTPUT CHANNELS CONFIG
bool RMSouthOverdriveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}


//GRAPHIC EDITOR CONFIG
bool RMSouthOverdriveAudioProcessor::hasEditor() const
{
    return true;
}


juce::AudioProcessorEditor* RMSouthOverdriveAudioProcessor::createEditor()
{
    return new RMSouthOverdriveAudioProcessorEditor (*this);
}


// LOAD AND SAVE STATE CONFIG
void RMSouthOverdriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}


void RMSouthOverdriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}


// CREATE ISTANCE
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RMSouthOverdriveAudioProcessor();
}


// CREATE PARAMETERS
juce::AudioProcessorValueTreeState::ParameterLayout RMSouthOverdriveAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;


    // DRIVE
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", juce::NormalisableRange<float> { 1.0f, 10.0f, }, 5.0f));
    
    //EQ
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BASS", "Bass", juce::NormalisableRange<float> { -5.0f, 5.0f, }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID", "Mid", juce::NormalisableRange<float> { -5.0f, 5.0f, }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TREBLE", "Treble", juce::NormalisableRange<float> { -5.0f, 5.0f, }, 0.0f));

    //VOLUME
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.0f, 1.0f, 0.75f));


    return { params.begin(), params.end() };
}