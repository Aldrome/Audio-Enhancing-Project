#pragma once
#include <array>
#include <cmath>
#include <juce_core/juce_core.h> // Needed for juce::Time

class SpeechDetector
{
public:
    void prepare(double sampleRate, int fftSize);
    void processFrame(const std::array<float, 512>& fftMagnitudes);
    bool isSpeechDetected() const;

private:
    float calculateEnergy(const std::array<float, 512>& magnitudes);
    float calculateSpectralEntropy(const std::array<float, 512>& magnitudes);

    // --- Thresholds for detection ---
    float energyThreshold = 0.0001f;
    float entropyThreshold = 2.0f;

    // --- Detection states ---
    bool speechDetected = false;
    bool lastSpeechDetected = false;

    // --- Sample rate (for future use) ---
    double currentSampleRate = 44100.0;

    // --- Smoothing and hysteresis ---
    float smoothingFactor = 1.0f;          // Controls response speed
    float speechDetectionHistory = 0.0f;   // Used for smoothing transitions

    // --- Hold mechanism ---
    juce::uint32 lastDetectionTimeMs = 0;        // Time of last detection
    const juce::uint32 holdDurationMs = 600;     // Hold detection "true" for 300ms
};
