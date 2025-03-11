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
    DBG("AudioRecorder destroyed");
}

void AudioRecorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    isRecording = true;
    fifoIndex = 0;
    currentSampleRate = sampleRate;
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
            float rawSample = buffer[sample];
            buffer[sample] *= volume.load();
            pushNextSampleIntoFifo(rawSample);
        }
    }
}

void AudioRecorder::releaseResources()
{
    isRecording = false;
}

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

void AudioRecorder::processFFT()
{
    windowFunction.multiplyWithWindowingTable(fftBuffer.data(), fftSize);
    fft.performRealOnlyForwardTransform(fftBuffer.data());

    std::array<float, 512> fftArray;
    std::copy(fftBuffer.begin(), fftBuffer.begin() + 512, fftArray.begin());

    if (onFFTDataReady)
        onFFTDataReady(fftArray);
}

void AudioRecorder::setVolume(float newVolume)
{
    volume.store(juce::Decibels::decibelsToGain(newVolume));
}
