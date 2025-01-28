#include "AudioRecorder.h"

AudioRecorder::AudioRecorder()
    : isRecording(false)  // Initialize the recording flag
{
    // Request microphone permission if required
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [this](bool granted) {
                if (granted)
                    setAudioChannels(1, 2);  // Enable 1 input channel, 2 output channels
                else
                    setAudioChannels(0, 2);  // No input, 2 output channels
            });
    }
    else
    {
        setAudioChannels(2, 2);  // Enable 2 input channel, 2 output channels
    }
}

AudioRecorder::~AudioRecorder()
{
    shutdownAudio();  // Shuts down the audio device and clears the audio source
}

void AudioRecorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    isRecording = true;  // Set the recording flag to true when starting
    juce::Logger::getCurrentLogger()->writeToLog("Preparing to play audio...");
    juce::Logger::getCurrentLogger()->writeToLog("Samples per block: " + juce::String(samplesPerBlockExpected));
    juce::Logger::getCurrentLogger()->writeToLog("Sample rate: " + juce::String(sampleRate));
}

void AudioRecorder::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!isRecording)
    {
        bufferToFill.clearActiveBufferRegion();  // Clear buffer if not recording
        return;
    }

    auto volumeScale = volume.load();

    // Retrieve the current audio device and active input channels
    auto* currentDevice = deviceManager.getCurrentAudioDevice();
    if (currentDevice == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    auto activeInputChannels = currentDevice->getActiveInputChannels();
    auto activeOutputChannels = currentDevice->getActiveOutputChannels();

    auto maxInputChannels = activeInputChannels.countNumberOfSetBits();
    auto maxOutputChannels = activeOutputChannels.countNumberOfSetBits();

    // Process the audio buffer
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        if (channel >= maxInputChannels || channel >= maxOutputChannels)
        {
            bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
            continue;
        }

        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            buffer[sample] *= volumeScale;
        }

        speechDSP.process(*bufferToFill.buffer);
    }
}

void AudioRecorder::releaseResources()
{
    isRecording = false;  // Set the recording flag to false when stopping
    juce::Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
}

void AudioRecorder::setVolume(float newVolume)
{
    float linearGain = juce::Decibels::decibelsToGain(newVolume);
    volume.store(linearGain);
}
