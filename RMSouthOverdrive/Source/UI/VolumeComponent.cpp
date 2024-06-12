/*
  ==============================================================================

    FilterComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================

// CONSTRUCTOR
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // SLIDERS RIGHT
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // CUTOFF SLIDER
    cutoffSlider.setLookAndFeel(&customLookAndFeelViolet);
    cutoffSlider.setName("Cutoff");
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setTextValueSuffix(" Hz");
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    cutoffSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    cutoffSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(235, 47, 144));
    addAndMakeVisible(&cutoffSlider);
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "LP_FILTERFREQ", cutoffSlider);

    // VOLUME SLIDER
    volumeSlider.setLookAndFeel(&customLookAndFeelViolet);
    volumeSlider.setName("Volume");
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(235, 47, 144));
    addAndMakeVisible(&volumeSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "VOLUME", volumeSlider);

    // LABELS
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f);

    // CUTOFF SLIDER
    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setFont(labelFont);
    cutoffLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(235, 47, 144));
    cutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&cutoffLabel);

    // VOLUME SLIDER
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setFont(labelFont);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(235, 47, 144));
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&volumeLabel);
}

// DESTRUCTOR
FilterComponent::~FilterComponent()
{
    cutoffSlider.setLookAndFeel(nullptr);
    volumeSlider.setLookAndFeel(nullptr);
}


// PAINT
void FilterComponent::paint (juce::Graphics& g)
{
}


// RESIZED
void FilterComponent::resized()
{
    const int margin = 25;
    const int sliderWidth = 75;
    const int sliderHeight = 75;
    const int labelHeight = 40;
    const int selectorWidth = 100;
    const int selectorHeight = 30;
    const int initialX = 0;
    const int yPosition = getHeight() - sliderHeight - 12;
    const int labelYPosition = yPosition - labelHeight;

    cutoffSlider.setBounds(0, yPosition, sliderWidth, sliderHeight);
    cutoffLabel.setBounds(0, labelYPosition, sliderWidth, labelHeight);

    volumeSlider.setBounds(initialX + sliderWidth + margin, yPosition, sliderWidth, sliderHeight);
    volumeLabel.setBounds(initialX + sliderWidth + margin, labelYPosition, sliderWidth, labelHeight);
}
