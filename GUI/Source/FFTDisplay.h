#pragma once

#include <JuceHeader.h>

class FFTDisplay : public juce::Component, public juce::Timer
{
public:
    FFTDisplay();
    ~FFTDisplay() override;

    void setFFTData(const std::array<float, 512>& newFFTData);

    void paint(juce::Graphics& g) override;
    void resized() override {}

    juce::Colour getSpectrogramColour(float magnitude);


private:
    static constexpr int fftSize = 512;
    static constexpr int widthSize = 512; // Number of time steps stored

    juce::Image spectrogramImage;
    int writeIndex = 0; // Circular buffer index

    void timerCallback() override;
    int getBinIndex(float frequency, float sampleRate) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTDisplay)
};
