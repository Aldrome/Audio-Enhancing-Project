#include "VolumeSlider.h"

// Constructor
VolumeSlider::VolumeSlider()
{
    // Initialize the slider
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    slider.setRange(0.0, 1.0, 0.01);  // Volume range from 0 to 1
    slider.setValue(0.5);             // Default value

    slider.textFromValueFunction = [](double value) {
        return juce::Decibels::toString(value); // Convert value to decibels for display
    };

    slider.valueFromTextFunction = [](const juce::String& text) {
        auto minusInfinitydB = -100.0;
        auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();
        return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB
             : decibelText.getDoubleValue();
    };

    // Attach a listener to detect changes
    slider.onValueChange = [this]() {
        if (onValueChange)
            onValueChange((float)slider.getValue());
    };

    addAndMakeVisible(slider);
}

// Destructor
VolumeSlider::~VolumeSlider() {};

void VolumeSlider::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::grey);
}

// Resized method
void VolumeSlider::resized()
{
    // Layout the slider in the component
    slider.setBounds(
        10,
        10,
        getWidth() - 20,
        getHeight() - 20
    );
}

float VolumeSlider::getValue() const
{
    return (float)slider.getValue();
}
