/*
  ==============================================================================

    CustomLookAndFeel.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel() {}

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

        // Draw the background circle
        g.setColour(juce::Colours::black); // Cerchio bianco
        g.fillEllipse(rx, ry, rw, rw);

        // Draw the filled arc (background arc)
        g.setColour(juce::Colours::white); // Arco di sfondo grigio chiaro
        juce::Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.8f);
        g.fillPath(filledArc);

        // Draw the filled arc (foreground arc)
        g.setColour(juce::Colours::red); // Indicatore rosso
        filledArc.clear();
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.8f);
        g.fillPath(filledArc);

        // Draw the thumb
        juce::Path thumb;
        const float thumbWidth = 4.0f;
        thumb.addRectangle(-thumbWidth / 2, -radius, thumbWidth, radius * 0.6f);
        g.setColour(juce::Colours::red); // Indicatore rosso
        g.fillPath(thumb, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }
};
