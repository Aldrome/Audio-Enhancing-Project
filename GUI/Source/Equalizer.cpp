#include "Equalizer.h"

Equalizer::Equalizer()
{
    const int numberOfEQslider = 10; // Equalizer slider count

    // Create vertical sliders for the equalizer
    for (int i = 0; i < numberOfEQslider; ++i)
    {
        auto slider = std::make_unique<juce::Slider>();
        slider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        slider->setRange(0.0, 1.0, 0.01);  // Set the range from 0 to 1
        slider->setValue(0.5);  // Default value
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);  // No text box
        addAndMakeVisible(slider.get());  // Add the slider to the component

        verticalSliders.push_back(std::move(slider));  // Store the slider in the vector
    }
}

Equalizer::~Equalizer() {}

// Paint method
void Equalizer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);  // Background color for the equalizer section
}

// Resized method
void Equalizer::resized()
{
    const int sliderWidth = 40;
    const int sliderHeight = getHeight();

    // Calculate the total width required for all sliders
    const int totalWidth = (sliderWidth * verticalSliders.size()) +  verticalSliders.size() - 1;

    // Calculate the starting X position to center the sliders
    const int startX = (getWidth() - totalWidth) / 2;

    // Set the bounds for each slider
    for (size_t i = 0; i < verticalSliders.size(); ++i)
    {
        verticalSliders[i]->setBounds(startX + sliderWidth * i, 0, sliderWidth, sliderHeight);
    }
}

