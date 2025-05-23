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
    // Don’t zero out any bins—let energy below 100 Hz through
    float energy = calculateEnergy(fftMagnitudes);
    float entropy = calculateSpectralEntropy(fftMagnitudes);

    bool currentDetection = (energy > energyThreshold)
        && (entropy > entropyThreshold);

    // Smoothing
    if (currentDetection)
        speechDetectionHistory = std::min(1.0f, speechDetectionHistory + smoothingFactor);
    else
        speechDetectionHistory = std::max(0.0f, speechDetectionHistory - smoothingFactor);

    // Hold logic
    const juce::uint32 now = juce::Time::getMillisecondCounter();
    if (speechDetectionHistory > 0.5f)
        lastDetectionTimeMs = now;

    speechDetected = (speechDetectionHistory > 0.5f)
        || (now - lastDetectionTimeMs < holdDurationMs);
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
