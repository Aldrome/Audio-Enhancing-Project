#pragma once

#include <JuceHeader.h>

class AutomaticGainControl
{
public:
    AutomaticGainControl();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    void setEnabled(bool shouldBeEnabled);
    bool isEnabled() const;

private:
    float targetRMS = 0.8f;           // Target output level (linear gain)
    float adjustmentSpeed = 0.001f;   // How fast the gain changes
    float currentGain = 1.0f;
    bool enabled = true;
    bool prepared = false;

    float computeRMS(const juce::dsp::AudioBlock<float>& block);
};
