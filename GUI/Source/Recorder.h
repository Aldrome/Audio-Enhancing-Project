#pragma once
#include <JuceHeader.h>

class Recorder : public juce::AudioAppComponent
{
public:
    Recorder();
    ~Recorder();

    void startRecordingToDefaultFile();
    void stopRecording();
    bool isRecording() const { return recording; }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Implementations of required methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;

private:
    void startRecording(const juce::File& file);
    void processAudio(const juce::AudioBuffer<float>& buffer);

    std::unique_ptr<juce::FileOutputStream> fileStream;
    bool recording = false;
};
