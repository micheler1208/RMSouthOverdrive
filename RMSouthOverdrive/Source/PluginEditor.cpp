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
    : AudioProcessorEditor(&p), 
    audioProcessor(p), 
    drive(audioProcessor.apvts), 
    EQ(audioProcessor.apvts), 
    volume(audioProcessor.apvts)
{
    // Window Size
    setSize(359, 650);
    
    // DRIVE ENABLE
    addAndMakeVisible(drive);

    // EQ ENABLE
    addAndMakeVisible(EQ);

    // VOLUME ENABLE
    addAndMakeVisible(volume);

    
    // Load IR Button
    loadIRButton.setButtonText("Load IR");
    loadIRButton.onClick = [this] {
        audioProcessor.getIR().loadImpulseResponse();
    };
    addAndMakeVisible(&loadIRButton);
}


// DESTRUCTOR
RMSouthOverdriveAudioProcessorEditor::~RMSouthOverdriveAudioProcessorEditor()
{    
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
}



// COMPONENTS - RESIZE
void RMSouthOverdriveAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);

    int sliderWidth = 80;
    int sliderHeight = 80;
    int spacing = 20;
    int extraLeftMargin = 25;
    int labelHeight = 20;

    int totalWidth = 5 * sliderWidth + 4 * spacing;
    int startX = (area.getWidth() - totalWidth) / 2 + extraLeftMargin;
    int startY = area.getHeight() - sliderHeight - labelHeight - 40;
    
    
    drive.setBounds(53, 50, getWidth() / 2, 150);

    EQ.setBounds(53, getHeight() -120, getWidth() , 150);

    volume.setBounds(getWidth()/2 + 53, 50, getWidth() / 2, 150);
    
    loadIRButton.setBounds(getWidth()/2-40, 380, 80, 30);

}