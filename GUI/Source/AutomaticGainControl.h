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
    float targetRMS = 0.8f;
    float adjustmentSpeed = 0.001f;    // Gain adjustment speed (fast attack)
    float releaseSpeed = 0.0001f;      // Gain release speed (slow release)
    float currentGain = 1.0f;
    float holdTimeSeconds = 2.0f;      // Hold time before releasing
    float detectionThreshold = 0.01f;  // RMS threshold to detect "loud" signal

    juce::Time lastLoudTime;

    bool enabled = true;
    bool prepared = false;

    float computeRMS(const juce::dsp::AudioBlock<float>& block);
};
