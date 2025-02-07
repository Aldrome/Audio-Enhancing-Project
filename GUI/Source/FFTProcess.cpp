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

const std::array<float, FFTProcess::fftSize / 2>& FFTProcess::getFFTData()
{
    fftReady = false; // Reset flag after retrieving data
    return fftBuffer;
}
