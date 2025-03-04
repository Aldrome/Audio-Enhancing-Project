#pragma once

#include <JuceHeader.h>
#include <vector>
#include <memory>

#include "AudioRecorder.h"
#include "Equalizer.h"
#include "FFTDisplay.h"
#include "VolumeSlider.h"

//==============================================================================
// MainComponent class definition
class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    std::vector<std::unique_ptr<juce::TextButton>> toggleButtons;

    std::vector<bool> isOnStates;

    void toggleButtonClicked(int buttonIndex);

    void handleFFTData(const std::array<float, 1024>& fftData);

    // Unique Pointers to classes
    std::unique_ptr<AudioRecorder> audioRecorder;
    std::unique_ptr<Equalizer> equalizer;
    std::unique_ptr<FFTDisplay> fftDisplay;
    std::unique_ptr<VolumeSlider> volumeSlider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
