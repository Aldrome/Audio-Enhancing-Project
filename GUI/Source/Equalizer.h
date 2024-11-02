#pragma once

#include <JuceHeader.h>


class Equalizer : public juce::Component
{
public:
    Equalizer(); // Constructor
    ~Equalizer() override; // Destructor

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    std::vector<std::unique_ptr<juce::Slider>> verticalSliders;  // Store the sliders

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Equalizer)
};
