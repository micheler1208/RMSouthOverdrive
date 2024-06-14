#pragma once

#include <JuceHeader.h>

class CustomLookAndFeelIR : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelIR() {}

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
        juce::Path diamondPath;

        float width = 50.0f;
        float height = 61.0f;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        auto center = bounds.getCentre();

        diamondPath.startNewSubPath(center.x, center.y - halfHeight); // Top
        diamondPath.lineTo(center.x + halfWidth, center.y);           // Right
        diamondPath.lineTo(center.x, center.y + halfHeight);          // Bottom
        diamondPath.lineTo(center.x - halfWidth, center.y);           // Left
        diamondPath.closeSubPath();

        juce::Colour brightRed = juce::Colour::fromRGB(255, 14, 14);
        juce::Colour fillColour = isMouseOverButton ? brightRed.withAlpha(0.35f) : juce::Colours::transparentBlack;

        g.setColour(fillColour);
        g.fillPath(diamondPath);

        // Disegna il bordo
        g.setColour(juce::Colours::black);
        g.strokePath(diamondPath, juce::PathStrokeType(0.30f));
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto font = juce::Font(16.0f);
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
            : juce::TextButton::textColourOffId));

        auto bounds = button.getLocalBounds();
        g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred, 1);
    }

    void drawCurvedLabel(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& text)
    {
        g.setFont(6.0f);
        g.setColour(juce::Colours::black);

        auto radius = bounds.getWidth() / 2.0f;
        auto center = bounds.getCentre();
        auto angleDelta = juce::MathConstants<float>::pi / text.length();
        auto angle = juce::MathConstants<float>::pi * 0.5f + angleDelta * (text.length() / 2.0f);

        for (auto i = 0; i < text.length(); ++i)
        {
            auto charWidth = g.getCurrentFont().getStringWidth(text.substring(i, i + 1)) / 2;
            auto x = center.x + (radius - charWidth) * std::cos(angle);
            auto y = center.y - (radius - charWidth) * std::sin(angle);
            g.addTransform(juce::AffineTransform::rotation(angle + juce::MathConstants<float>::halfPi, x, y));
            g.drawSingleLineText(text.substring(i, i + 1), x - charWidth, y);
            g.addTransform(juce::AffineTransform::rotation(-(angle + juce::MathConstants<float>::halfPi), x, y));
            angle -= angleDelta;
        }
    }
};
