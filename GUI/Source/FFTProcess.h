#pragma once
#include <JuceHeader.h>
#include <vector>

class FFTProcess
{
public:
    static constexpr int fftOrder = 10; // 2^10 = 1024-point FFT
    static constexpr int fftSize = 1 << fftOrder;

    FFTProcess();
    ~FFTProcess();

    void pushSample(float sample);
    bool isFFTReady() const;
    const std::array<float, fftSize / 2>& getFFTData();

private:
    void performFFT();

    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;

    std::vector<float> circularBuffer;
    std::array<float, fftSize / 2> fftBuffer{};
    int writeIndex = 0;
    int numSamplesCollected = 0;
    bool fftReady = false;
};
