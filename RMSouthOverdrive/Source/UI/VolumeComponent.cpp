/*
  ==============================================================================

    VolumeComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VolumeComponent.h"

//==============================================================================

// CONSTRUCTOR
VolumeComponent::VolumeComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // VOLUME SLIDER
    volumeSlider.setLookAndFeel(&customLookAndFeelDriveVolume);
    volumeSlider.setName("Volume");
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&volumeSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "VOLUME", volumeSlider);

    // LABELS
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f);

    // VOLUME SLIDER
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setFont(labelFont);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(248, 255, 209));
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&volumeLabel);
}

// DESTRUCTOR
VolumeComponent::~VolumeComponent()
{
    volumeSlider.setLookAndFeel(nullptr);
}


// PAINT
void VolumeComponent::paint (juce::Graphics& g)
{
}


// RESIZED
void VolumeComponent::resized()
{
    volumeLabel.setBounds(0, 0, 75, 40);
    volumeSlider.setBounds(0, 40, 75, 75);
}
