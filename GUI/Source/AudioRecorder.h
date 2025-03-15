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
    void setFilterEnabled(bool enabled);

    bool isRecording = false;

    std::function<void(const std::array<float, 512>&)> onFFTDataReady;

private:
    std::atomic<float> volume{ 1.0f };

    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;

    juce::dsp::WindowingFunction<float> windowFunction{ fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris };
    juce::dsp::FFT fft{ fftOrder };
    juce::AbstractFifo fifo{ fftSize };

    std::vector<float> fftBuffer;
    std::vector<float> fifoBuffer;
    int fifoIndex = 0;
    double currentSampleRate = 44100.0;

    //------------------------------------------------------
    // Filters
    bool isFilterEnabled = true;

    // Bandpass filter to keep speech range
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bandpassFilter;

    // High-shelf EQ to add brightness
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highShelfFilter;

    // Multi-band expander to restore dynamics in high frequencies
    juce::dsp::Compressor<float> highBandExpander;

    void pushNextSampleIntoFifo(float sample);
    void processFFT();
    void updateFilters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioRecorder)
};
