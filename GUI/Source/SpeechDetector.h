#pragma once
#include <array>
#include <vector>
#include <cmath>
#include <juce_core/juce_core.h>  // For juce::Time

class SpeechDetector
{
public:
    void prepare(double sampleRate, int fftSize);
    void processFrame(const std::array<float, 512>& fftMagnitudes);
    bool isSpeechDetected() const;

private:
    float calculateEnergy(const std::array<float, 512>& magnitudes);
    float calculateSpectralEntropy(const std::array<float, 512>& magnitudes);

    float energyThreshold = 0.01f;
    float entropyThreshold = 4.0f;

    bool speechDetected = false;
    double currentSampleRate = 44100.0;

    // Hold mechanism
    juce::uint32 lastDetectionTimeMs = 0;
    const juce::uint32 holdDurationMs = 500;  // Hold detection for 500ms
};
