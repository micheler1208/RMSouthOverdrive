/*
  ==============================================================================

    PluginEditor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR
RMSouthOverdriveAudioProcessorEditor::RMSouthOverdriveAudioProcessorEditor (RMSouthOverdriveAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Dimensioni iniziali dell'editor
    setSize(600, 400);

    // Slider per il Drive
    driveSlider.setLookAndFeel(&customLookAndFeel);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setRange(0.0, 10.0, 0.1);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    driveSlider.setValue(audioProcessor.getDrive());
    addAndMakeVisible(&driveSlider);
    driveSlider.addListener(this);

    // Slider per il Bass
    bassSlider.setLookAndFeel(&customLookAndFeel);
    bassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bassSlider.setRange(-5.0, 5.0, 0.1); // Range modificato
    bassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    bassSlider.setValue(audioProcessor.getBass());
    addAndMakeVisible(&bassSlider);
    bassSlider.addListener(this);

    // Slider per il Mid
    midSlider.setLookAndFeel(&customLookAndFeel);
    midSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    midSlider.setRange(-5.0, 5.0, 0.1); // Range modificato
    midSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    midSlider.setValue(audioProcessor.getMid());
    addAndMakeVisible(&midSlider);
    midSlider.addListener(this);

    // Slider per il Treble
    trebleSlider.setLookAndFeel(&customLookAndFeel);
    trebleSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trebleSlider.setRange(-5.0, 5.0, 0.1); // Range modificato
    trebleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    trebleSlider.setValue(audioProcessor.getTreble());
    addAndMakeVisible(&trebleSlider);
    trebleSlider.addListener(this);

    // Slider per il Volume di Uscita
    outputVolumeSlider.setLookAndFeel(&customLookAndFeel);
    outputVolumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputVolumeSlider.setRange(0.0, 1.0, 0.01);
    outputVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    outputVolumeSlider.setValue(audioProcessor.getOutputVolume());
    addAndMakeVisible(&outputVolumeSlider);
    outputVolumeSlider.addListener(this);
    
    // Pulsante per caricare l'IR
    loadIRButton.setButtonText("Load IR");
    loadIRButton.onClick = [this] { audioProcessor.loadImpulseResponse(); };
    addAndMakeVisible(&loadIRButton);
}



// DESTRUCTOR
RMSouthOverdriveAudioProcessorEditor::~RMSouthOverdriveAudioProcessorEditor()
{
    // Reset LookAndFeel to default
    driveSlider.setLookAndFeel(nullptr);
    bassSlider.setLookAndFeel(nullptr);
    midSlider.setLookAndFeel(nullptr);
    trebleSlider.setLookAndFeel(nullptr);
    outputVolumeSlider.setLookAndFeel(nullptr);
    
}


// GUI - PAINT
void RMSouthOverdriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);
    if (!backgroundImage.isNull())
    {
        g.drawImageAt(backgroundImage, 0, 0);
    }
    else
    {
        // Fallback color
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    // Set the font for the title text
    juce::Font titleFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::HMRocking_ttf, BinaryData::HMRocking_ttfSize));
    titleFont.setHeight(48.0f);
    g.setFont(titleFont);
    g.setColour(juce::Colours::white);
    g.drawFittedText("RM - SOUTH OVERDRIVE", getLocalBounds().removeFromTop(70), juce::Justification::centred, 1);

    // Set the font for the labels
    juce::Font customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::HMRocking_ttf, BinaryData::HMRocking_ttfSize));
    customFont.setHeight(24.0f);
    g.setFont(customFont);
    g.setColour(juce::Colours::white);


    // Draw labels under each slider
    auto area = getLocalBounds().reduced(28); // Provide some margin
    int labelHeight = 20;
    int sliderWidth = 80; // Same width as the sliders
    int sliderHeight = 80;
    int spacing = 20; // Space between sliders
    int extraLeftMargin = 33; // Extra margin on the left

    int totalWidth = 5 * sliderWidth + 4 * spacing; // Total width of sliders and spaces
    int startX = (area.getWidth() - totalWidth) / 2 + extraLeftMargin; // Calculate the start X position to center sliders
    int startY = area.getHeight() - sliderHeight - 40; // Position sliders at the bottom of the interface

    // Center the labels under the sliders
    g.drawFittedText("Drive", juce::Rectangle<int>(startX, startY + sliderHeight, sliderWidth, labelHeight), juce::Justification::centred, 1);
    g.drawFittedText("Bass", juce::Rectangle<int>(startX + sliderWidth + spacing, startY + sliderHeight, sliderWidth, labelHeight), juce::Justification::centred, 1);
    g.drawFittedText("Mid", juce::Rectangle<int>(startX + 2 * (sliderWidth + spacing), startY + sliderHeight, sliderWidth, labelHeight), juce::Justification::centred, 1);
    g.drawFittedText("Treble", juce::Rectangle<int>(3 + startX + 3 * (sliderWidth + spacing), startY + sliderHeight, sliderWidth, labelHeight), juce::Justification::centred, 1);
    g.drawFittedText("Output", juce::Rectangle<int>(startX + 4 * (sliderWidth + spacing), startY + sliderHeight, sliderWidth, labelHeight), juce::Justification::centred, 1);
}



// COMPONENTS - RESIZE
void RMSouthOverdriveAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20); // Provide some margin

    int sliderWidth = 80; // Larghezza di ciascun slider
    int sliderHeight = 80; // Altezza di ciascun slider
    int spacing = 20; // Spazio tra gli slider
    int extraLeftMargin = 25; // Aggiungi un margine extra sul lato sinistro
    int labelHeight = 20; // Altezza dell'etichetta

    int totalWidth = 5 * sliderWidth + 4 * spacing; // Larghezza totale degli slider e degli spazi
    int startX = (area.getWidth() - totalWidth) / 2 + extraLeftMargin; // Calcola l'inizio dell'area degli slider per centrarli
    int startY = area.getHeight() - sliderHeight - labelHeight - 40; // Posiziona gli slider nella parte bassa dell'interfaccia
    
    loadIRButton.setBounds(startX + 2 * (sliderWidth + spacing), startY + 80, sliderWidth, 30);

    driveSlider.setBounds(startX, startY, sliderWidth, sliderHeight);
    bassSlider.setBounds(startX + sliderWidth + spacing, startY, sliderWidth, sliderHeight);
    midSlider.setBounds(startX + 2 * (sliderWidth + spacing), startY, sliderWidth, sliderHeight);
    trebleSlider.setBounds(startX + 3 * (sliderWidth + spacing), startY, sliderWidth, sliderHeight);
    outputVolumeSlider.setBounds(startX + 4 * (sliderWidth + spacing), startY, sliderWidth, sliderHeight);
}


// SLIDER CHANGE
void RMSouthOverdriveAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &driveSlider)
    {
        audioProcessor.setDrive(driveSlider.getValue());
    }
    else if (slider == &outputVolumeSlider)
    {
        audioProcessor.setOutputVolume(outputVolumeSlider.getValue());
    }
    else if (slider == &bassSlider)
    {
        audioProcessor.setBass(bassSlider.getValue());
    }
    else if (slider == &midSlider)
    {
        audioProcessor.setMid(midSlider.getValue());
    }
    else if (slider == &trebleSlider)
    {
        audioProcessor.setTreble(trebleSlider.getValue());
    }

    // Update filter coefficients whenever a slider value changes
    audioProcessor.updateFilterCoefficients();
}
