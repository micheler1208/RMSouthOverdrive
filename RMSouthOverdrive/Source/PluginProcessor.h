#pragma once
#include <JuceHeader.h>

class RM5150HarmonicEnhancerAudioProcessor : public juce::AudioProcessor
{
public:
    RM5150HarmonicEnhancerAudioProcessor();
    ~RM5150HarmonicEnhancerAudioProcessor() override;

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

    // Parameter getters and setters
    float getDrive() const;
    void setDrive(float newDrive);
    float getOutputVolume() const;
    void setOutputVolume(float newVolume);
    float getBass() const;
    void setBass(float newBass);
    float getMid() const;
    void setMid(float newMid);
    float getTreble() const;
    void setTreble(float newTreble);

    // Update filter coefficients method (new)
    void updateFilterCoefficients();
    
    // Custom method to check if the bus layout is supported
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    
    // Sample for Smoothing Filter
    float prevSample;  // Variabile membro per il filtro di smoothing    

    
private:
    // Functions and Filters
    float softClipping(float x);
    float hardClipping(float x);
    float saturation(float x);
    float smoothingFilter(float currentSample, float& prevSample, float smoothingFactor);
    void applyFilter(juce::AudioBuffer<float>& buffer);

    // DSP filters
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    using FilterDuplicator = juce::dsp::ProcessorDuplicator<Filter, Coefficients>;

    FilterDuplicator highPassFilter;
    FilterDuplicator lowPassFilter;

    juce::dsp::ProcessorChain<FilterDuplicator, FilterDuplicator, FilterDuplicator> eqChain;

    // Audio parameters
    std::atomic<float>* drive;
    std::atomic<float>* outputVolume;
    std::atomic<float>* bass;
    std::atomic<float>* mid;
    std::atomic<float>* treble;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RM5150HarmonicEnhancerAudioProcessor)
};
