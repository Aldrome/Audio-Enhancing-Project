#pragma once

#include <JuceHeader.h>

class FFTDisplay : public juce::Component, private juce::Timer
{
public:
    FFTDisplay();
    ~FFTDisplay() override;

    void setFFTData(const std::array<float, 512>& newFFTData);

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    static constexpr int fftSize = 512;
    static constexpr int widthSize = 512;

    juce::Image spectrogramImage;
    std::array<float, fftSize> fftData{};
    int writeIndex = 0;

    void timerCallback() override;
    void drawSpectrogram(juce::Graphics& g);
    void drawSpectrum(juce::Graphics& g);
    void drawGrid(juce::Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTDisplay)
};
