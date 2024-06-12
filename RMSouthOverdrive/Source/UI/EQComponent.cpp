/*
  ==============================================================================

    AdsrComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================

// CONSTRUCTOR
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // SLIDERS LEFT
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // ATTACK SLIDER
    attackSlider.setLookAndFeel(&customLookAndFeelYellow);
    attackSlider.setName("Attack");
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setRange(0.01, 2000.0, 0.01);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    attackSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    attackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&attackSlider);
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);

    // DECAY SLIDER
    decaySlider.setLookAndFeel(&customLookAndFeelYellow);
    decaySlider.setName("Decay");
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setRange(0.01, 2000.0, 0.01);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    decaySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&decaySlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);

    // SUSTAIN SLIDER
    sustainSlider.setLookAndFeel(&customLookAndFeelYellow);
    sustainSlider.setName("Sustain");
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setRange(-5.0, 5.0, 0.01);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    sustainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    sustainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&sustainSlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);

    // RELEASE SLIDER
    releaseSlider.setLookAndFeel(&customLookAndFeelYellow);
    releaseSlider.setName("Release");
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setRange(1.0, 3000.0, 0.01);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    releaseSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    releaseSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&releaseSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);

    // LABELS
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f);

    // ATTACK LABEL
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setFont(labelFont);
    attackLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    attackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&attackLabel);

    // DECAY LABEL
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setFont(labelFont);
    decayLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&decayLabel);

    // SUSTAIN LABEL
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setName("Sustain"); // Imposta il nome qui
    sustainLabel.setFont(labelFont);
    sustainLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    sustainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&sustainLabel);

    // RELEASE LABEL
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setFont(labelFont);
    releaseLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    releaseLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&releaseLabel);
}

// DESTRUCTOR
AdsrComponent::~AdsrComponent()
{
    attackSlider.setLookAndFeel(nullptr);
    decaySlider.setLookAndFeel(nullptr);
    sustainSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
}

// PAINT
void AdsrComponent::paint (juce::Graphics& g)
{
}

// RESIZED
void AdsrComponent::resized()
{
    const int margin = 25;
    const int sliderWidth = 75;
    const int sliderHeight = 75;
    const int initialX = 0;
    const int yPosition = 40;
    const int labelHeight = 40;
    const int labelYPosition = yPosition -40;

    attackSlider.setBounds(initialX, yPosition, sliderWidth, sliderHeight);
    decaySlider.setBounds(initialX + sliderWidth + margin, yPosition, sliderWidth, sliderHeight);
    sustainSlider.setBounds(initialX + 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    releaseSlider.setBounds(initialX + 3 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);

    attackLabel.setBounds(initialX, labelYPosition, sliderWidth, labelHeight);
    decayLabel.setBounds(initialX + sliderWidth + margin, labelYPosition, sliderWidth, labelHeight);
    sustainLabel.setBounds(initialX + 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);
    releaseLabel.setBounds(initialX + 3 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

}
