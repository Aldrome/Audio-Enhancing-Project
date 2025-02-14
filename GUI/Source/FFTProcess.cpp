#include "FFTProcess.h"

FFTProcess::FFTProcess()
    : fft(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann),
    circularBuffer(fftSize, 0.0f) // Initialize buffer with zeros
{
    fftBuffer.fill(0.0f);
}

FFTProcess::~FFTProcess() 
{

}

void FFTProcess::setSampleRate(double rate)
{
    sampleRate = rate;
}

void FFTProcess::setSpeechThreshold(float threshold)
{
    speechThreshold = threshold;
}

void FFTProcess::pushSample(float sample)
{
    // Store the sample in the circular buffer
    circularBuffer[writeIndex] = sample;
    writeIndex = (writeIndex + 1) % fftSize; // Wrap around index

    numSamplesCollected++;

    // Process FFT when enough new samples are collected
    if (numSamplesCollected >= fftSize)
    {
        performFFT();
        numSamplesCollected = 0; // Reset counter
    }
}

void FFTProcess::performFFT()
{
    // Apply window function and copy data from circular buffer to FFT buffer
    for (int i = 0; i < fftSize; ++i)
    {
        int readIndex = (writeIndex + i) % fftSize;
        fftBuffer[i] = circularBuffer[readIndex];
    }

    // Apply window function
    window.multiplyWithWindowingTable(fftBuffer.data(), fftSize);

    // Perform FFT
    fft.performFrequencyOnlyForwardTransform(fftBuffer.data());

    fftReady = true;
}

bool FFTProcess::isFFTReady() const
{
    return fftReady;
}

bool FFTProcess::detectSpeech()
{
    if (!fftReady) return false; // No new FFT data yet

    float energy = 0.0f;
    int binStart = static_cast<int>(300.0 / (sampleRate / fftSize));  // Approx 300 Hz bin
    int binEnd = static_cast<int>(4000.0 / (sampleRate / fftSize));   // Approx 4 kHz bin

    for (int i = binStart; i < binEnd; ++i)
    {
        energy += fftBuffer[i] * fftBuffer[i]; // Power spectrum (magnitude squared)
    }

    energy = std::sqrt(energy / (binEnd - binStart)); // Normalize energy

    return energy > speechThreshold; // Simple thresholding
}

const std::array<float, FFTProcess::fftSize / 2>& FFTProcess::getFFTData()
{
    fftReady = false; // Reset flag after retrieving data
    return fftBuffer;
}