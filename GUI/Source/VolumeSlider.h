#pragma once

#include <JuceHeader.h>

class VolumeSlider : public juce::Component
{
public:
    VolumeSlider(); // Constructor declaration
    ~VolumeSlider() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Accessor for the slider's value
    float getValue() const;

    // Callback to notify on value changes
    std::function<void(float)> onValueChange;

private:
    juce::Slider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeSlider)
};
