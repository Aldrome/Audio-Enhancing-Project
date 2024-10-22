#pragma once

#include <JuceHeader.h>
#include <vector>
#include <memory>

#include "ToggleSlider.h"

//==============================================================================
class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Vector to hold multiple toggle buttons using unique_ptr
    std::vector<std::unique_ptr<juce::TextButton>> toggleButtons;

    // Vector to track the state of each button
    std::vector<bool> isOnStates;

    // Button click handler
    void toggleButtonClicked(int buttonIndex);

    // Vector to hold multiple vertical sliders using unique_ptr
    std::vector<std::unique_ptr<juce::Slider>> verticalSliders;

    std::unique_ptr<ToggleSlider> animatedButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
