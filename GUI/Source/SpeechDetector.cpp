#include "SpeechDetector.h"

void SpeechDetector::prepare(double sampleRate, int fftSize)
{
    currentSampleRate = sampleRate;
    energyThreshold = 0.01f;
    entropyThreshold = 4.0f;
    
    // Smoothing parameters
    smoothingFactor = 0.1f;  // Lower is smoother, higher is more responsive
    lastSpeechDetected = false;
    speechDetectionHistory = 0.0f;
}

void SpeechDetector::processFrame(const std::array<float, 512>& fftMagnitudes)
{
    std::array<float, 512> filteredMagnitudes = fftMagnitudes;
    for (int i = 0; i < 10; ++i)
        filteredMagnitudes[i] = 0.0f; // Filter low frequencies

    float energy = calculateEnergy(filteredMagnitudes);
    float entropy = calculateSpectralEntropy(filteredMagnitudes);

    bool currentDetection = (energy > energyThreshold) && (entropy > entropyThreshold);

    // Apply smoothing/hysteresis
    if (currentDetection)
    {
        speechDetectionHistory = std::min(1.0f, speechDetectionHistory + smoothingFactor); // Gradual increase
    }
    else
    {
        speechDetectionHistory = std::max(0.0f, speechDetectionHistory - smoothingFactor); // Gradual decrease
    }

    // Determine speech detection state with hysteresis
    if (speechDetectionHistory > 0.5f)  // A threshold to decide when speech is detected
    {
        speechDetected = true;
    }
    else
    {
        speechDetected = false;
    }
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
