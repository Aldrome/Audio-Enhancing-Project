#pragma once

#include <JuceHeader.h>

#include "SpeechDetector.h"

class AudioRecorder : public juce::AudioAppComponent
{
public:
    AudioRecorder();
    ~AudioRecorder() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void setVolume(float newVolume);
    void setFilterEnabled(bool enabled);

    std::function<void(const std::array<float, 512>&)> onFFTDataReady; // Callback for FFT data

    bool isSpeechDetected() const { return speechDetector.isSpeechDetected(); }
private:
    SpeechDetector speechDetector;

    void updateFilters();
    void pushNextSampleIntoFifo(float sample);
    void processFFT();

    // Audio processing state
    bool isRecording;
    bool isFilterEnabled = true;
    double currentSampleRate = 44100.0;
    std::atomic<float> volume{ 1.0f };

    // FFT Processing
    static constexpr int fftOrder = 10;  // 2^10 = 1024 FFT size
    static constexpr int fftSize = 1 << fftOrder;

    juce::dsp::FFT fft{ fftOrder };
    juce::dsp::WindowingFunction<float> windowFunction{ fftSize, juce::dsp::WindowingFunction<float>::hann };

    std::vector<float> fftBuffer;
    std::vector<float> fifoBuffer;
    int fifoIndex = 0;

    // Filters
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highShelfFilter;

    // Multi-band Expander
    juce::dsp::Compressor<float> highBandExpander;
};
