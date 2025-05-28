#include "EnergyDetector.h"

void EnergyDetector::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    sampleBuffer.clear();
    sampleBuffer.reserve(bufferSize);
}

void EnergyDetector::processSample(float sample)
{
    sampleBuffer.push_back(sample);

    if (sampleBuffer.size() >= bufferSize)
    {
        float energy = calculateEnergy(sampleBuffer);
        if (energy > energyThreshold)
            lastDetectionTimeMs = juce::Time::getMillisecondCounter();

        sampleBuffer.clear();
    }

    juce::uint32 now = juce::Time::getMillisecondCounter();
    energyDetected = (now - lastDetectionTimeMs) < holdDurationMs;
}

void EnergyDetector::processBlock(const juce::AudioBuffer<float>& buffer)
{
    float energy = calculateEnergy(buffer);
    if (energy > energyThreshold)
        lastDetectionTimeMs = juce::Time::getMillisecondCounter();

    juce::uint32 now = juce::Time::getMillisecondCounter();
    energyDetected = (now - lastDetectionTimeMs) < holdDurationMs;
}

float EnergyDetector::calculateEnergy(const juce::AudioBuffer<float>& buffer)
{
    float totalEnergy = 0.0f;
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* data = buffer.getReadPointer(channel);
        for (int i = 0; i < numSamples; ++i)
            totalEnergy += data[i] * data[i];
    }

    return totalEnergy / (numSamples * numChannels);
}

float EnergyDetector::calculateEnergy(const std::vector<float>& samples)
{
    float totalEnergy = 0.0f;
    for (float s : samples)
        totalEnergy += s * s;

    return totalEnergy / samples.size();
}

bool EnergyDetector::isEnergyDetected() const
{
    return energyDetected;
}
