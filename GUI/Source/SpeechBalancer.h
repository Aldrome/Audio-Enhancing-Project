#pragma once

#include <JuceHeader.h>

// The SpeechBalancer handles both noise gating and compression to enhance speech clarity.
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

private:
    // Noise gate: treats it as a compressor with inverse logic
    juce::dsp::Compressor<float> noiseGate;
    juce::dsp::Compressor<float> compressor;

    bool prepared = false;
};
