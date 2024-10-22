/*
  ==============================================================================

    ToggleSlider.h
    Toggle Slider for ON and OFF, including 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ToggleSlider : public juce::Button, public juce::Timer
{
public:
    ToggleSlider();
    ~ToggleSlider();

    void paint(juce::Graphics& g) override;
    void clicked() override;
    void timerCallback() override;

private:
    float animationProgress;
    bool isAnimating;
};
