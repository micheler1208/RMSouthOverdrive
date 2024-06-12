/*
  ==============================================================================

    CustomLookAndFeelEQ.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeelEQ : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelEQ()
    {
        customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::caviar_ttf, BinaryData::caviar_ttfSize));
    }

    // ROTARY SLIDER
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Background of the circle
        g.setColour(juce::Colour::fromRGBA(116, 29, 12, 0));
        g.fillEllipse(rx, ry, rw, rw);

        // Arc of the circle
        g.setColour(juce::Colour::fromRGB(255, 237, 175));
        juce::Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.8f);
        g.fillPath(filledArc);

        // Filled Arc
        g.setColour(juce::Colour::fromRGB(223, 135, 45));
        filledArc.clear();
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.8f);
        g.fillPath(filledArc);

        // Thumb - Pointer
        juce::Path thumb;
        const float thumbWidth = 4.0f;
        thumb.addRectangle(-thumbWidth / 2, -radius, thumbWidth, radius * 0.6f);
        g.setColour(juce::Colour::fromRGB(223, 135, 45));
        g.fillPath(thumb, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }


    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(customFont);
        g.drawFittedText(label.getText(), label.getLocalBounds(), label.getJustificationType(), 2);
    }

    void drawTextEditorOutline (juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {
        if (textEditor.isEnabled())
        {
            if (textEditor.hasKeyboardFocus (true) && ! textEditor.isReadOnly())
            {
                g.setColour (textEditor.findColour (juce::TextEditor::focusedOutlineColourId));
                g.drawRect (0, 0, width, height, 2);
            }
            else
            {
                g.setColour (textEditor.findColour (juce::TextEditor::outlineColourId));
                g.drawRect (0, 0, width, height);
            }
        }
    }

private:
    juce::Font customFont;
};
