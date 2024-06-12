/*
  ==============================================================================

    CustomSlider.h
    Author:  micheler1208

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider()
    {
        setNumDecimalPlacesToDisplay(1);
    }

    void setDecimalPlaces(int places)
    {
        numDecimalPlaces = places;
        setNumDecimalPlacesToDisplay(places);
    }

protected:
    juce::String getTextFromValue(double value) override
    {
        juce::String suffix;
        juce::String textValue;

        if (getName() == "Drive" || getName() == "Bass" || getName() == "Mid" || getName() == "Trble")
        {
            suffix = " ";
            textValue = juce::String(value, numDecimalPlaces);
        }
        else if ( getName() == "Volume")
        {
            suffix = " %";
            textValue = juce::String(value * 100.0, 0);
        }

        return textValue + suffix;
    }

    double getValueFromText(const juce::String& text) override
    {
        auto valueStr = text.upToFirstOccurrenceOf(" ", false, false);
        auto value = valueStr.getDoubleValue();

        if (getName() == "Volume")
        {
            value /= 100.0;
        }

        return value;
    }

private:
    int numDecimalPlaces = 1;
};
