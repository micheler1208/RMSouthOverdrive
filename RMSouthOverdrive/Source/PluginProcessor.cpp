/*
  ==============================================================================

    RM5150 - PROCESSOR

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

// Soft Clipping Function
float RM5150HarmonicEnhancerAudioProcessor::softClipping(float x)
{
    return (3.0f / 2.0f) * x - (1.0f / 2.0f) * x * x * x;
}

// Hard Clipping Function
float RM5150HarmonicEnhancerAudioProcessor::hardClipping(float x)
{
    if (x > 1.0f) return 1.0f;
    else if (x < -1.0f) return -1.0f;
    else return x;
}

// Saturation Function
float RM5150HarmonicEnhancerAudioProcessor::saturation(float x)
{
    return tanh(x);
}

// Smoothing Filter
float RM5150HarmonicEnhancerAudioProcessor::smoothingFilter(float currentSample, float& prevSample, float smoothingFactor)
{
    float smoothedSample = smoothingFactor * prevSample + (1.0f - smoothingFactor) * currentSample;
    prevSample = smoothedSample;
    return smoothedSample;
}


// Funzione per applicare i filtri
void RM5150HarmonicEnhancerAudioProcessor::applyFilter(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    highPassFilter.process(context);
    lowPassFilter.process(context);
}


// CONSTRUCTOR
RM5150HarmonicEnhancerAudioProcessor::RM5150HarmonicEnhancerAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput("Input", juce::AudioChannelSet::mono(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    drive = new std::atomic<float>(1.0f);
    outputVolume = new std::atomic<float>(0.7f);
    bass = new std::atomic<float>(0.0f);
    mid = new std::atomic<float>(0.0f);
    treble = new std::atomic<float>(0.0f);
}




// DESTRUCTOR
RM5150HarmonicEnhancerAudioProcessor::~RM5150HarmonicEnhancerAudioProcessor()
{
    delete drive;
    delete outputVolume;
    delete bass;
    delete mid;
    delete treble;
}


// GET NAME
const juce::String RM5150HarmonicEnhancerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

// MIDI CONFIG
bool RM5150HarmonicEnhancerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RM5150HarmonicEnhancerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RM5150HarmonicEnhancerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

// TAIL AUDIO CONFIG
double RM5150HarmonicEnhancerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// PROGRAM CONFIG
int RM5150HarmonicEnhancerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RM5150HarmonicEnhancerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RM5150HarmonicEnhancerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RM5150HarmonicEnhancerAudioProcessor::getProgramName (int index)
{
    return {};
}

void RM5150HarmonicEnhancerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}



// INITIALIZATION CONFIG
void RM5150HarmonicEnhancerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = getTotalNumOutputChannels();

    eqChain.prepare(spec);
    highPassFilter.prepare(spec);
    lowPassFilter.prepare(spec);

    // Imposta i coefficienti del filtro
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 40.0f);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 20000.0f);

    updateFilterCoefficients();
}


// PAUSE/STOP CONFIG
void RM5150HarmonicEnhancerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}


// OUTPUT CHANNELS CONFIG
bool RM5150HarmonicEnhancerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}



// PROCESS CONFIG
void RM5150HarmonicEnhancerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clears any unused channels just in case
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    // Process input with distortion
    if (totalNumInputChannels > 0 && totalNumOutputChannels > 1) {
        auto* inputChannelData = buffer.getReadPointer(0);
        auto* leftOutputChannelData = buffer.getWritePointer(0);
        auto* rightOutputChannelData = buffer.getWritePointer(1);
        
        // Apply a power function to the output volume for finer control at lower values
        float adjustedOutputVolume = std::pow(*outputVolume, 5.0f); // Power of 2, you can adjust this value


        for (int sample = 0; sample < numSamples; ++sample) {
            float cleanSignal = inputChannelData[sample];

            // First stage of distortion
            float gainStage1 = cleanSignal * *drive;
            float clippedSignal1 = softClipping(gainStage1);

            // Smoothing between stages
            float smoothedSignal1 = smoothingFilter(clippedSignal1, prevSample, 0.1f);

            // Second stage of distortion
            float gainStage2 = smoothedSignal1 * *drive;
            float clippedSignal2 = saturation(gainStage2);

            // Smoothing between stages
            float smoothedSignal2 = smoothingFilter(clippedSignal2, prevSample, 0.1f);

            // Third stage of distortion
            float gainStage3 = smoothedSignal2 * *drive;
            float clippedSignal3 = hardClipping(gainStage3);

            // Apply adjusted output volume
            float processedSignal = clippedSignal3 * adjustedOutputVolume;

            // Duplicate processed signal to both output channels
            leftOutputChannelData[sample] = processedSignal;
            rightOutputChannelData[sample] = processedSignal;
        }
        
        // Applica i filtri high-pass e low-pass
        applyFilter(buffer);

        // Create a new audio block from the buffer
        juce::dsp::AudioBlock<float> block(buffer);

        // Apply EQ to the processed buffer
        eqChain.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
}



void RM5150HarmonicEnhancerAudioProcessor::updateFilterCoefficients()
{
    auto sampleRate = getSampleRate();

    // Apply a power function to make the knobs less sensitive and adjust to ±5 dB range
    float adjustedBass = *bass * std::abs(*bass) / 5.0f;
    float adjustedMid = *mid * std::abs(*mid) / 5.0f;
    float adjustedTreble = *treble * std::abs(*treble) / 5.0f;

    // BASS (Low Shelf Filter)
    float bassGainDB = adjustedBass;  // ±5 dB range
    DBG("Bass Gain (dB): " << bassGainDB);
    auto bassCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 30.0f, 1.0f, std::pow(10.0f, bassGainDB / 20.0f));
    *eqChain.get<0>().state = *bassCoefficients;

    // MID (Peaking Filter)
    float midGainDB = adjustedMid;  // ±5 dB range
    DBG("Mid Gain (dB): " << midGainDB);
    auto midCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 650.0f, 1.0f, std::pow(10.0f, midGainDB / 20.0f));
    *eqChain.get<1>().state = *midCoefficients;

    // TREBLE (High Shelf Filter)
    float trebleGainDB = adjustedTreble;  // ±5 dB range
    DBG("Treble Gain (dB): " << trebleGainDB);
    auto trebleCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 10000.0f, 1.0f, std::pow(10.0f, trebleGainDB / 20.0f));
    *eqChain.get<2>().state = *trebleCoefficients;
}





//GRAPHIC EDITOR CONFIG
bool RM5150HarmonicEnhancerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RM5150HarmonicEnhancerAudioProcessor::createEditor()
{
    return new RM5150HarmonicEnhancerAudioProcessorEditor (*this);
}

// LOAD AND SAVE STATE CONFIG
void RM5150HarmonicEnhancerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RM5150HarmonicEnhancerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// GETTER AND SETTER
float RM5150HarmonicEnhancerAudioProcessor::getDrive() const { return *drive; }
void RM5150HarmonicEnhancerAudioProcessor::setDrive(float newDrive) { *drive = newDrive; }

float RM5150HarmonicEnhancerAudioProcessor::getOutputVolume() const { return *outputVolume; }
void RM5150HarmonicEnhancerAudioProcessor::setOutputVolume(float newVolume) { *outputVolume = newVolume; }

float RM5150HarmonicEnhancerAudioProcessor::getBass() const { return *bass; }
void RM5150HarmonicEnhancerAudioProcessor::setBass(float newBass) { *bass = newBass; }

float RM5150HarmonicEnhancerAudioProcessor::getMid() const { return *mid; }
void RM5150HarmonicEnhancerAudioProcessor::setMid(float newMid) { *mid = newMid; }

float RM5150HarmonicEnhancerAudioProcessor::getTreble() const { return *treble; }
void RM5150HarmonicEnhancerAudioProcessor::setTreble(float newTreble) { *treble = newTreble; }


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RM5150HarmonicEnhancerAudioProcessor();
}