#pragma once

#include <JuceHeader.h>
#include "SpeechDetector.h"
#include "SpeechBalancer.h"
#include "EnergyDetector.h"

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
    void setSpeechBalancerEnabled(bool enabled);


    std::function<void(const std::array<float, 512>&)> onFFTDataReady;

    bool isSpeechDetected() const { return speechDetector.isSpeechDetected(); }
    bool isEnergyDetected() const { return energyDetector.isEnergyDetected(); }

private:
    SpeechDetector speechDetector;
    EnergyDetector energyDetector;
    SpeechBalancer speechBalancer;

    void updateFilters();
    void pushNextSampleIntoFifo(float sample);
    void processFFT();

    bool isRecording;
    bool filterEnabled = true;
    bool speechBalancerEnabled = true;
    double currentSampleRate = 44100.0;
    std::atomic<float> volume{ 1.0f };

    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;

    juce::dsp::FFT fft{ fftOrder };
    juce::dsp::WindowingFunction<float> windowFunction{ fftSize, juce::dsp::WindowingFunction<float>::hann };

    std::vector<float> fftBuffer;
    std::vector<float> fifoBuffer;
    int fifoIndex = 0;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highShelfFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> midBoostFilter;

    // juce::dsp::Compressor<float> highBandExpander;
};
