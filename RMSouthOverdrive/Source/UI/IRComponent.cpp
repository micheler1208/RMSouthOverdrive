#include "IRComponent.h"

// CONSTRUCTOR
IRComponent::IRComponent(juce::AudioProcessorValueTreeState& apvt, RMSouthOverdriveAudioProcessor& processor)
    : audioProcessor(processor)
{
    loadIRButton.setLookAndFeel(&customLookAndFeelIR);
    loadIRButton.onClick = [this] {
        audioProcessor.getIR().loadImpulseResponse();
        };

    IRLabel.setText("Load Impulse Response", juce::dontSendNotification);
    IRLabel.setLookAndFeel(&customLookAndFeelIR);

    auto fontStream = juce::Font::getDefaultSansSerifFontName();
    customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));

    IRLabel.setFont(customFont.withHeight(26.0f));
    IRLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 244, 195));

    addAndMakeVisible(loadIRButton);
    addAndMakeVisible(IRLabel);
}

// DESTRUCTOR
IRComponent::~IRComponent()
{
    loadIRButton.setLookAndFeel(nullptr);
    IRLabel.setLookAndFeel(nullptr);
}

void IRComponent::paint(juce::Graphics& g)
{
}

// RESIZED
void IRComponent::resized()
{
    loadIRButton.setBounds(40, 0, 68, 68);
    IRLabel.setBounds(0, 62, 150, 30);
}
