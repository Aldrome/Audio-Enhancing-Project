#include "AudioRecorder.h"

AudioRecorder::AudioRecorder()
    : isRecording(false),
    fftBuffer(fftSize, 0.0f),
    fifoBuffer(fftSize, 0.0f)
{
    setAudioChannels(2, 2);
}

AudioRecorder::~AudioRecorder()
{
    shutdownAudio();
}

void AudioRecorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    isRecording = true;
    fifoIndex = 0;
}

void AudioRecorder::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!isRecording)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            buffer[sample] *= volume.load(); // Apply volume scaling
            pushNextSampleIntoFifo(buffer[sample]);
        }
    }
}

void AudioRecorder::releaseResources()
{
    isRecording = false;
}

// Pushes samples into FIFO and triggers FFT when ready
void AudioRecorder::pushNextSampleIntoFifo(float sample)
{
    fifoBuffer[fifoIndex++] = sample;

    if (fifoIndex == fftSize)
    {
        std::copy(fifoBuffer.begin(), fifoBuffer.end(), fftBuffer.begin());
        fifoIndex = 0;
        processFFT();
    }
}

// Processes FFT on the collected samples
void AudioRecorder::processFFT()
{
    windowFunction.multiplyWithWindowingTable(fftBuffer.data(), fftSize);
    fft.performRealOnlyForwardTransform(fftBuffer.data());

    // Convert fftBuffer to std::array<float, 1024>
    std::array<float, 1024> fftArray;
    std::copy(fftBuffer.begin(), fftBuffer.begin() + 1024, fftArray.begin());

    // Send FFT data to MainComponent
    if (onFFTDataReady)
        onFFTDataReady(fftArray);
}

void AudioRecorder::setVolume(float newVolume)
{
    volume.store(juce::Decibels::decibelsToGain(newVolume));
}
