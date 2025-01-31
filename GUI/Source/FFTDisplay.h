#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FFTDisplay  : public juce::Component, public juce::Timer
{
public:
    FFTDisplay();
    ~FFTDisplay() override;

    void setFFTData(const std::array<float, 1024>& newFFTData);

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    static constexpr int fftSize = 1024;
    std::array<float, fftSize> fftData;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTDisplay)
};
