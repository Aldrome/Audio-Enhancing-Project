#pragma once

#include <JuceHeader.h>

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
    void setGain(float newGain);

private:
    float gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeechDSP)
};
