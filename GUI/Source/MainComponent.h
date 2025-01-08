#pragma once

#include <JuceHeader.h>
#include <vector>
#include <memory>

#include "Equalizer.h"
<<<<<<< Updated upstream
#include "Recorder.h"
=======
#include "AudioRecorder.h"
>>>>>>> Stashed changes

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
    // Vector to hold multiple toggle buttons using unique_ptr
    std::vector<std::unique_ptr<juce::TextButton>> toggleButtons;

    // Vector to track the state of each button
    std::vector<bool> isOnStates;

    // Button click handler
    void toggleButtonClicked(int buttonIndex);

    // Unique pointer for the Equalizer component
    std::unique_ptr<Equalizer> equalizer;

<<<<<<< Updated upstream
    std::unique_ptr<Recorder> recorder;
    juce::TextButton startRecordingButton{ "Start Recording" };
    juce::TextButton stopRecordingButton{ "Stop Recording" };
=======
    // The AudioRecorder instance
    std::unique_ptr<AudioRecorder> audioRecorder;  // Pointer for the AudioRecorder instance

>>>>>>> Stashed changes

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
