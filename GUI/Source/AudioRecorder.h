#pragma once

#include <JuceHeader.h>
#include <atomic>

class AudioRecorder : public juce::AudioAppComponent
{
public:
    explicit AudioRecorder();
    ~AudioRecorder() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void setVolume(float newVolume);

    bool isRecording = false;

    std::function<void(const std::array<float, 1024>&)> onFFTDataReady;

private:
    std::atomic<float> volume{ 1.0f };


    // FFT Processing
    static constexpr int fftOrder = 11;  // 2^11 = 2048 FFT size
    static constexpr int fftSize = 1 << fftOrder;

    juce::dsp::WindowingFunction<float> windowFunction{ fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris };
    juce::dsp::FFT fft{ fftOrder };
    juce::AbstractFifo fifo{ fftSize };
    std::vector<float> fftBuffer;
    std::vector<float> fifoBuffer;
    int fifoIndex = 0;

    void pushNextSampleIntoFifo(float sample);
    void processFFT();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioRecorder)
};
