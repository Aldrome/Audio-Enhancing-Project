#pragma once

#include <JuceHeader.h>

#include "FFTProcess.h"

//==============================================================================
/*
*/
class SpeechDSP  : public juce::Component
{
public:
    SpeechDSP();
    ~SpeechDSP() override;

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);

    void setSampleRate(double rate) { fftProcess.setSampleRate(rate); }
    void setSpeechThreshold(float threshold) { fftProcess.setSpeechThreshold(threshold); }

private:
    FFTProcess fftProcess;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeechDSP)
};
