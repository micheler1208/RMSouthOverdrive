#include "IRComponent.h"

// CONSTRUCTOR
IRComponent::IRComponent(juce::AudioProcessorValueTreeState& apvt, RMSouthOverdriveAudioProcessor& processor)
    : audioProcessor(processor)
{
    loadIRButton.setLookAndFeel(&customLookAndFeelIR);
    loadIRButton.onClick = [this] {
        audioProcessor.getIR().loadImpulseResponse();
        };

    curvedLabel.setText("Load Impulse Response", juce::dontSendNotification);
    curvedLabel.setLookAndFeel(&customLookAndFeelIR);

    addAndMakeVisible(loadIRButton);
    addAndMakeVisible(curvedLabel);
}

// DESTRUCTOR
IRComponent::~IRComponent()
{
    loadIRButton.setLookAndFeel(nullptr);
    curvedLabel.setLookAndFeel(nullptr);
}

void IRComponent::paint(juce::Graphics& g)
{
    auto bounds = loadIRButton.getBounds().toFloat();
    customLookAndFeelIR.drawCurvedLabel(g, bounds.toNearestInt(), curvedLabel.getText());
}

// RESIZED
void IRComponent::resized()
{
    loadIRButton.setBounds(0, 0, 68, 68);
}
