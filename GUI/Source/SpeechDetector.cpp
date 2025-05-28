#include "SpeechDetector.h"

// Checks FFT magnitudes for speech presence using energy and spectral entropy

void SpeechDetector::prepare(double sampleRate, int fftSize)
{
    currentSampleRate = sampleRate;
    energyThreshold = 0.01f;
    entropyThreshold = 4.0f;

    lastDetectionTimeMs = 0;
}

void SpeechDetector::processFrame(const std::array<float, 512>& fftMagnitudes)
{
    std::array<float, 512> filteredMagnitudes = fftMagnitudes;
    for (int i = 0; i < 10; ++i)
        filteredMagnitudes[i] = 0.0f;

    float energy = calculateEnergy(filteredMagnitudes);
    float entropy = calculateSpectralEntropy(filteredMagnitudes);

    // Check immediate detection
    bool immediateDetection = (energy > energyThreshold) && (entropy > entropyThreshold);

    // If immediate detection occurs, reset lastDetectionTimeMs
    if (immediateDetection)
        lastDetectionTimeMs = juce::Time::getMillisecondCounter();

    // Maintain detection if within holdDurationMs
    juce::uint32 now = juce::Time::getMillisecondCounter();
    if ((now - lastDetectionTimeMs) < holdDurationMs)
        speechDetected = true;
    else
        speechDetected = false;
}

bool SpeechDetector::isSpeechDetected() const
{
    return speechDetected;
}

float SpeechDetector::calculateEnergy(const std::array<float, 512>& magnitudes)
{
    float energy = 0.0f;
    for (float mag : magnitudes)
        energy += mag * mag;

    return energy / static_cast<float>(magnitudes.size());
}

float SpeechDetector::calculateSpectralEntropy(const std::array<float, 512>& magnitudes)
{
    float totalPower = 0.0f;
    for (float mag : magnitudes)
        totalPower += mag;

    if (totalPower <= 0.0f)
        return 0.0f;

    float entropy = 0.0f;
    for (float mag : magnitudes)
    {
        float p = mag / totalPower;
        if (p > 0.0f)
            entropy -= p * std::log2(p);
    }

    return entropy;
}
