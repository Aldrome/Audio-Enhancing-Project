#include "SpeechDetector.h"

void SpeechDetector::prepare(double sampleRate, int fftSize)
{
    currentSampleRate = sampleRate;
    // Thresholds can be tuned based on actual input
    energyThreshold = 0.01f;
    entropyThreshold = 4.0f;
}

void SpeechDetector::processFrame(const std::array<float, 512>& fftMagnitudes)
{
    // Optionally filter low frequencies (wind) by ignoring first ~10 bins (~100–150 Hz)
    std::array<float, 512> filteredMagnitudes = fftMagnitudes;
    for (int i = 0; i < 10; ++i)
        filteredMagnitudes[i] = 0.0f;

    float energy = calculateEnergy(filteredMagnitudes);
    float entropy = calculateSpectralEntropy(filteredMagnitudes);

    speechDetected = (energy > energyThreshold) && (entropy > entropyThreshold);
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
