#pragma once
#include <array>
#include <vector>
#include <cmath>

class SpeechDetector
{
public:
    void prepare(double sampleRate, int fftSize);
    void processFrame(const std::array<float, 512>& fftMagnitudes);
    bool isSpeechDetected() const;

private:
    float calculateEnergy(const std::array<float, 512>& magnitudes);
    float calculateSpectralEntropy(const std::array<float, 512>& magnitudes);

    // New members for smoothing and hysteresis
    float energyThreshold = 0.01f;
    float entropyThreshold = 4.0f;

    bool speechDetected = false;
    double currentSampleRate = 44100.0;

    // Smoothing-related variables
    float smoothingFactor = 0.1f;  // Lower is smoother, higher is more responsive
    float speechDetectionHistory = 0.0f;  // Used to smooth detection state
    bool lastSpeechDetected = false;
};
