#pragma once

#include <JuceHeader.h>
#include "AutomaticGainControl.h"

// The SpeechBalancer handles both noise gating and compression to enhance speech clarity.
// Contains most DSP features and linked through here
class SpeechBalancer
{
public:
    SpeechBalancer();
    ~SpeechBalancer() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);

    void setNoiseGateThreshold(float dB);
    void setCompressorThreshold(float dB);
    void setCompressorRatio(float ratio);
    void setAttackRelease(float attackMs, float releaseMs);

    void setEnabled(bool shouldEnable);
    bool isEnabled() const noexcept;

private:
    // Noise gate: treats it as a compressor with inverse logic
    juce::dsp::Compressor<float> noiseGate;
    juce::dsp::Compressor<float> compressor;

    AutomaticGainControl agc;

    bool prepared = false;
    bool enabled = true;  // NEW: default enabled
};
