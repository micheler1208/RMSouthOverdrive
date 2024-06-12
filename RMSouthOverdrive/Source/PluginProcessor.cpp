/*
  ==============================================================================

    PluginProcessor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

// Soft Clipping Function
float RMSouthOverdriveAudioProcessor::softClipping(float x)
{
    const float a = 2.0f;
    const float b = 1.0f;
    return (a * x) / (1 + b * std::abs(x));
}

// Hard Clipping Function
float RMSouthOverdriveAudioProcessor::hardClipping(float x)
{
    const float positiveThreshold = 0.8f;
    const float negativeThreshold = -0.8f;
    if (x > positiveThreshold) return positiveThreshold;
    else if (x < negativeThreshold) return negativeThreshold;
    else return x;
}

// Diode Clipping
float RMSouthOverdriveAudioProcessor::diodeClipping(float x) {
    const float threshold = 0.6f; // Tensione di soglia di un diodo al silicio
    if (x > threshold) return threshold;
    else if (x < -threshold) return -threshold;
    else return x - (std::pow(x, 3) / 3);
}

// Combined Clipping
float RMSouthOverdriveAudioProcessor::combinedClipping(float x) {
    float softClipped = softClipping(x);
    return hardClipping(softClipped);
}


// Saturation Function
float RMSouthOverdriveAudioProcessor::saturation(float x)
{
    return std::atan(x);
}

// Smoothing Filter
float RMSouthOverdriveAudioProcessor::smoothingFilter(float currentSample, float& prevSample, float smoothingFactor)
{
    float smoothedSample = smoothingFactor * prevSample + (1.0f - smoothingFactor) * currentSample;
    prevSample = smoothedSample;
    return smoothedSample;
}


// Funzione per applicare i filtri
void RMSouthOverdriveAudioProcessor::applyFilter(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    highPassFilter.process(context);
    lowPassFilter.process(context);
}


// CONSTRUCTOR
RMSouthOverdriveAudioProcessor::RMSouthOverdriveAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput("Input", juce::AudioChannelSet::mono(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "PARAMETERS", createParams())
{
    drive = new std::atomic<float>(5.0f);
    outputVolume = new std::atomic<float>(0.75f);
    bass = new std::atomic<float>(0.0f);
    mid = new std::atomic<float>(0.0f);
    treble = new std::atomic<float>(0.0f);
}




// DESTRUCTOR
RMSouthOverdriveAudioProcessor::~RMSouthOverdriveAudioProcessor()
{
    delete drive;
    delete outputVolume;
    delete bass;
    delete mid;
    delete treble;
    
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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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



// INITIALIZATION CONFIG
void RMSouthOverdriveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = getTotalNumOutputChannels();

    eqChain.prepare(spec);
    highPassFilter.prepare(spec);
    lowPassFilter.prepare(spec);
    convolutionProcessor.prepare(spec);

    // Imposta i coefficienti del filtro
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 100.0f);
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 11000.0f);

    updateFilterCoefficients();
}


// PAUSE/STOP CONFIG
void RMSouthOverdriveAudioProcessor::releaseResources()
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


// NEW PROCESS BLOCK
void RMSouthOverdriveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    if (totalNumInputChannels > 0 && totalNumOutputChannels > 1) {
        auto* inputChannelData = buffer.getReadPointer(0);
        auto* leftOutputChannelData = buffer.getWritePointer(0);
        auto* rightOutputChannelData = buffer.getWritePointer(1);

        float adjustedOutputVolume = std::pow(*outputVolume, 5.0f);

        for (int sample = 0; sample < numSamples; ++sample) {
            float cleanSignal = inputChannelData[sample];

            // First stage of distortion
            float gainStage1 = cleanSignal * *drive;
            float clippedSignal1 = combinedClipping(gainStage1);

            // Smoothing between stages
            float smoothedSignal1 = smoothingFilter(clippedSignal1, prevSample, 0.1f);

            // Second stage of distortion
            float gainStage2 = smoothedSignal1 * *drive;
            float clippedSignal2 = diodeClipping(gainStage2);

            // Smoothing between stages
            float smoothedSignal2 = smoothingFilter(clippedSignal2, prevSample, 0.1f);

            // Third stage of distortion
            float gainStage3 = smoothedSignal2 * *drive;
            float clippedSignal3 = hardClipping(gainStage3);

            // Apply low-pass filter TO REMOVE
            float filteredSignal = clippedSignal3;

            // Apply adjusted output volume
            float processedSignal = filteredSignal * adjustedOutputVolume;

            // Duplicate processed signal to both output channels
            leftOutputChannelData[sample] = processedSignal;
            rightOutputChannelData[sample] = processedSignal;
        }

        applyFilter(buffer);
        
        // Process IR convolution
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        convolutionProcessor.process(context);

        eqChain.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
}


//LOAD IMPULSE RESPONSE
void RMSouthOverdriveAudioProcessor::loadImpulseResponse()
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


//UPDATE FILTERS EQ
void RMSouthOverdriveAudioProcessor::updateFilterCoefficients()
{
    auto sampleRate = getSampleRate();

    // Apply a power function to make the knobs less sensitive and adjust to ±5 dB range
    float adjustedBass = *bass * std::abs(*bass) / 5.0f;
    float adjustedMid = *mid * std::abs(*mid) / 5.0f;
    float adjustedTreble = *treble * std::abs(*treble) / 5.0f;

    // BASS (Peak Filter)
    float bassGainDB = adjustedBass;  // ±5 dB range
    DBG("Bass Gain (dB): " << bassGainDB);
    auto bassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 100.0f, 0.707f, std::pow(10.0f, bassGainDB / 20.0f));
    *eqChain.get<0>().state = *bassCoefficients;

    // MID (Peaking Filter)
    float midGainDB = adjustedMid;  // ±5 dB range
    DBG("Mid Gain (dB): " << midGainDB);
    auto midCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 650.0f, 0.707f, std::pow(10.0f, midGainDB / 20.0f));
    *eqChain.get<1>().state = *midCoefficients;

    // TREBLE (Peak Filter)
    float trebleGainDB = adjustedTreble;  // ±5 dB range
    DBG("Treble Gain (dB): " << trebleGainDB);
    auto trebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 2800.0f, 0.707f, std::pow(10.0f, trebleGainDB / 20.0f));
    *eqChain.get<2>().state = *trebleCoefficients;
}





//GRAPHIC EDITOR CONFIG
bool RMSouthOverdriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RMSouthOverdriveAudioProcessor::createEditor()
{
    return new RMSouthOverdriveAudioProcessorEditor (*this);
}

// LOAD AND SAVE STATE CONFIG
void RMSouthOverdriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RMSouthOverdriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// GETTER AND SETTER
float RMSouthOverdriveAudioProcessor::getDrive() const { return *drive; }
void RMSouthOverdriveAudioProcessor::setDrive(float newDrive) { *drive = newDrive; }

float RMSouthOverdriveAudioProcessor::getOutputVolume() const { return *outputVolume; }
void RMSouthOverdriveAudioProcessor::setOutputVolume(float newVolume) { *outputVolume = newVolume; }

float RMSouthOverdriveAudioProcessor::getBass() const { return *bass; }
void RMSouthOverdriveAudioProcessor::setBass(float newBass) { *bass = newBass; }

float RMSouthOverdriveAudioProcessor::getMid() const { return *mid; }
void RMSouthOverdriveAudioProcessor::setMid(float newMid) { *mid = newMid; }

float RMSouthOverdriveAudioProcessor::getTreble() const { return *treble; }
void RMSouthOverdriveAudioProcessor::setTreble(float newTreble) { *treble = newTreble; }


//==============================================================================
// This creates new instances of the plugin..
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