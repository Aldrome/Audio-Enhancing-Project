#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FFTProcess  : public juce::Component
{
public:
    static constexpr int fftSize = 512;  // Adjust as needed

    FFTProcess();
    ~FFTProcess() override;

    void processFFT(juce::AudioBuffer<float>& buffer);

private:
    void pushNextSampleIntoFifo(float sample);

    static constexpr int fftOrder = 10; // 2^10 = 1024
    static constexpr int fftSize = 1 << fftOrder;

    juce::dsp::FFT forwardFFT{ fftOrder };
    juce::dsp::WindowingFunction<float> window{ fftSize, juce::dsp::WindowingFunction<float>::hann };

    std::array<float, fftSize> fifo = { 0 };
    std::array<float, fftSize> fftData = { 0 };
    int fifoIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTProcess)
};
