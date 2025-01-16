#pragma once

#include <JuceHeader.h>
#include <atomic>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AudioRecorder : public juce::AudioAppComponent
{
public:
    //==============================================================================
    explicit AudioRecorder();
    ~AudioRecorder() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void setVolume(float newVolume);

    //==============================================================================
    bool isRecording = false;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Random random;
    std::atomic<float> volume{ 1.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioRecorder)
};