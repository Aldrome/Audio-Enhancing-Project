#pragma once
#include <JuceHeader.h>

class EnergyDetector
{
public:
    EnergyDetector() = default;

    void prepare(double sampleRate);
    void processSample(float sample);
    void processBlock(const juce::AudioBuffer<float>& buffer);

    bool isEnergyDetected() const;

private:
    float calculateEnergy(const juce::AudioBuffer<float>& buffer);
    float calculateEnergy(const std::vector<float>& samples);

    double currentSampleRate = 44100.0;
    float energyThreshold = 0.01f;
    bool energyDetected = false;
    juce::uint32 lastDetectionTimeMs = 0;
    const int holdDurationMs = 500; // Extend detection for 300 ms

    std::vector<float> sampleBuffer;
    static constexpr int bufferSize = 512; // Collect samples for energy calc
};
