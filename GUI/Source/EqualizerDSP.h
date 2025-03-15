/*
  ==============================================================================

    EqualizerDSP.h
    Created: 15 Mar 2025 1:25:36pm
    Author:  ASUS

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EqualizerDSP  : public juce::Component
{
public:
    EqualizerDSP();
    ~EqualizerDSP() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqualizerDSP)
};
