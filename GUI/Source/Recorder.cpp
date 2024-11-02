#include "Recorder.h"

// Constructor
Recorder::Recorder() {
    // Set up audio device
    setAudioChannels(2, 0);
}

// Destructor
Recorder::~Recorder() {
    if (recording) {
        stopRecording();
    }
    shutdownAudio();
}

// Start recording to a default file on the desktop
void Recorder::startRecordingToDefaultFile() {
    juce::File recordingFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("recording.wav");
    startRecording(recordingFile);
}

// Start recording to a specified file
void Recorder::startRecording(const juce::File& file) {
    if (recording) {
        return;
    }

    fileStream = file.createOutputStream();

    if (fileStream.get() == nullptr) {
        juce::Logger::writeToLog("Failed to open file for recording.");
        return;
    }

    recording = true;
    juce::Logger::writeToLog("Recording started: " + file.getFullPathName());
}

// Stop recording and close the file
void Recorder::stopRecording() {
    if (!recording) {
        return;
    }

    recording = false;
    fileStream.reset();
    juce::Logger::writeToLog("Recording stopped.");
}

// Get audio data from the input device and process it
void Recorder::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (recording) {
        processAudio(*bufferToFill.buffer); // Process and save audio data to file
    }
}

// Process audio data and write it to the file stream
void Recorder::processAudio(const juce::AudioBuffer<float>& buffer) {
    if (!recording || fileStream.get() == nullptr) {
        return;
    }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* data = buffer.getReadPointer(channel);
        fileStream->write(data, buffer.getNumSamples() * sizeof(float));
    }
}

// Implement the prepareToPlay method
void Recorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // Prepare for playback and processing (e.g., allocating buffers, etc.)
}

// Implement the releaseResources method
void Recorder::releaseResources() {
    // Free any resources that were allocated in prepareToPlay
}