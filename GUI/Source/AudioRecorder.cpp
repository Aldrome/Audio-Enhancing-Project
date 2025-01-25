#include "AudioRecorder.h"

AudioRecorder::AudioRecorder()
    : isRecording(false)  // Initialize the recording flag
{
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }
}

AudioRecorder::~AudioRecorder()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void AudioRecorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    isRecording = true;  // Set the recording flag to true when starting
    juce::String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    juce::Logger::getCurrentLogger()->writeToLog(message);
}

void AudioRecorder::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!isRecording) {
        bufferToFill.clearActiveBufferRegion();  // Clear buffer if not recording
        return;
    }

    auto volumeScale = volume.load() *2.0f;

    // If recording, fill the buffer with noise
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            buffer[sample] = random.nextFloat() * volumeScale - volume.load();
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