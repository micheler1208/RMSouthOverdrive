/*
  ==============================================================================

    AdsrComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EQComponent.h"

//==============================================================================

// CONSTRUCTOR
EQComponent::EQComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;


    // Bass Slider
    bassSlider.setLookAndFeel(&customLookAndFeelEQ);
    bassSlider.setName("Bass");
    bassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bassSlider.setRange(-5.0, 5.0, 0.1);
    bassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&bassSlider);
    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "BASS", bassSlider);

    // Mid Slider
    midSlider.setLookAndFeel(&customLookAndFeelEQ);
    midSlider.setName("Mid");
    midSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midSlider.setRange(-5.0, 5.0, 0.1);
    midSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&midSlider);
    midAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "MID", midSlider);

    // Treble Slider
    trebleSlider.setLookAndFeel(&customLookAndFeelEQ);
    trebleSlider.setName("Treble");
    trebleSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trebleSlider.setRange(-5.0, 5.0, 0.1);
    trebleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&trebleSlider);
    trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "TREBLE", trebleSlider);
    

    // LABELS
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f);

    // Bass Label
    bassLabel.setText("Bass", juce::dontSendNotification);
    bassLabel.setName("Bass");
    bassLabel.setFont(labelFont);
    bassLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    bassLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&bassLabel);

    // Mid Label
    midLabel.setText("Mid", juce::dontSendNotification);
    midLabel.setName("Mid");
    midLabel.setFont(labelFont);
    midLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    midLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&midLabel);

    // Treble Label
    trebleLabel.setText("Treble", juce::dontSendNotification);
    trebleLabel.setName("Treble");
    trebleLabel.setFont(labelFont);
    trebleLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    trebleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&trebleLabel);
}

// DESTRUCTOR
EQComponent::~EQComponent()
{
    bassSlider.setLookAndFeel(nullptr);
    midSlider.setLookAndFeel(nullptr);
    trebleSlider.setLookAndFeel(nullptr);
}

// PAINT
void EQComponent::paint (juce::Graphics& g)
{
}

// RESIZED
void EQComponent::resized()
{
    const int margin = 25;
    const int sliderWidth = 75;
    const int sliderHeight = 75;
    const int initialX = 0;
    const int yPosition = 40;
    const int labelHeight = 40;
    const int labelYPosition = yPosition -40;

    bassSlider.setBounds(initialX, yPosition, sliderWidth, sliderHeight);
    midSlider.setBounds(initialX + sliderWidth + margin, yPosition, sliderWidth, sliderHeight);
    trebleSlider.setBounds(initialX + 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);

    bassLabel.setBounds(initialX, labelYPosition, sliderWidth, labelHeight);
    midLabel.setBounds(initialX + sliderWidth + margin, labelYPosition, sliderWidth, labelHeight);
    trebleLabel.setBounds(initialX + 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

}
