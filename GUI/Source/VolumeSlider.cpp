#include "VolumeSlider.h"

// Constructor
VolumeSlider::VolumeSlider()
{
    // Initialize the slider
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);

    // Set the range for decibels
    slider.setRange(-100.0, 6.0, 0.1);  // dB range: -100 dB to 6 dB, with 0.1 dB steps
    slider.setValue(-10.0);             // Default value (-10 dB)

    // Convert numeric value to decibel string for display
    slider.textFromValueFunction = [](double value) {
        if (value <= -100.0)
            return juce::String("-INF");                // Negative infinite for -100 dB
        return juce::Decibels::toString(value); // Convert value to decibels (1 decimal)
        };

    // Convert decibel text input to numeric value
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
VolumeSlider::~VolumeSlider() {}

// Paint method
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
