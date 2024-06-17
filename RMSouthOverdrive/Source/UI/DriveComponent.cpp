/*
  ==============================================================================

    DriveComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DriveComponent.h"

//==============================================================================

// CONSTRUCTOR
DriveComponent::DriveComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // DRIVE SLIDER
    driveSlider.setLookAndFeel(&customLookAndFeelDriveVolume);
    driveSlider.setName("Drive");
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setRange(1.0, 5.0, 0.1);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&driveSlider);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "DRIVE", driveSlider);
    
    // LABELS
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f);

    // DRIVE LABEL
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setFont(labelFont);
    driveLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(248, 255, 209));
    driveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&driveLabel);
}

// DESTRUCTOR
DriveComponent::~DriveComponent()
{
    driveSlider.setLookAndFeel(nullptr);
}


// PAINT
void DriveComponent::paint (juce::Graphics& g)
{
}


// RESIZED
void DriveComponent::resized()
{
    driveLabel.setBounds(0, 0, 75, 40);
    driveSlider.setBounds(0, 40, 75, 75);
}
