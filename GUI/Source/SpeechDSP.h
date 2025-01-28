/*
  ==============================================================================

    SpeechDSP.h
    Created: 28 Jan 2025 9:04:41pm
    Author:  ASUS

  ==============================================================================
*/

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

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpeechDSP)
};
