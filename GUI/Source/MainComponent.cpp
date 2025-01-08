#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    setSize(600, 600);

    addAndMakeVisible(startRecordingButton);
    startRecordingButton.onClick = [this]() { recorder->startRecordingToDefaultFile(); };

    addAndMakeVisible(stopRecordingButton);
    stopRecordingButton.onClick = [this]() { recorder->stopRecording(); };

    recorder = std::make_unique<Recorder>();


    const int numberOfButtons = 3;  // Number of buttons

    // Initialize states for buttons
    isOnStates.resize(numberOfButtons, false);  // Initialize all states to OFF (false)
    audioRecorder = std::make_unique<AudioRecorder>();

    // Create the buttons
    for (int i = 0; i < numberOfButtons; ++i)
    {
        auto button = std::make_unique<juce::TextButton>();
        button->setButtonText("OFF");  // Initial state

        // Capture 'this' and 'i' properly
        button->onClick = [this, i]() {
            DBG("Button " + std::to_string(i) + " clicked"); // Debugging output to confirm the button click
            toggleButtonClicked(i);
            };

        addAndMakeVisible(button.get());
        toggleButtons.push_back(std::move(button));
    }

    // Create and add the Equalizer
    equalizer = std::make_unique<Equalizer>();
    addAndMakeVisible(equalizer.get());

    resized();  // Call resized to set up positions
}

MainComponent::~MainComponent()
{
    if (recorder->isRecording()) {
        recorder->stopRecording();
    }
}

//==============================================================================
// Paint method for drawing the main UI
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(50.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Audio Enhancement Project", getLocalBounds(), juce::Justification::centredTop, true);
}

//==============================================================================
// Resized method for laying out child components
void MainComponent::resized()
{
    const int buttonWidth = 100;
    const int buttonHeight = 50;
    const int buttonSpacing = 20;

    // Layout the buttons
    for (size_t i = 0; i < toggleButtons.size(); ++i)
    {
        toggleButtons[i]->setBounds(20, 80 + (buttonHeight + buttonSpacing) * i, buttonWidth, buttonHeight);
    }

    // Set the bounds for the equalizer component
    if (equalizer)  // Ensure equalizer is valid
    {
        equalizer->setBounds(20, 250, getWidth() - 40, 300);  // Position the equalizer below the buttons
    }

    // Layout for buttons
    startRecordingButton.setBounds(10, 10, 120, 30);
    stopRecordingButton.setBounds(140, 10, 120, 30);
}

//==============================================================================
// Toggle button click handler
void MainComponent::toggleButtonClicked(int buttonIndex)
{
    if (buttonIndex >= 0 && buttonIndex < isOnStates.size()) // Check for valid index
    {
        DBG("Toggling button " + std::to_string(buttonIndex)); // Debugging output

        // Toggle the state of the button
        isOnStates[buttonIndex] = !isOnStates[buttonIndex];

        // Audio Recording Button
        if (buttonIndex == 0)  // Assuming buttonIndex 0 controls recording
        {
            if (isOnStates[buttonIndex])  // Button is ON
            {
                if (!audioRecorder->isRecording)  // Avoid redundant calls to start recording
                {
                    DBG("Starting recording...");
                    audioRecorder->prepareToPlay(512, 44100.0);  // Initialize the recorder
                    DBG("Recording started");
                }
            }
            else  // Button is OFF
            {
                if (audioRecorder->isRecording)  // Avoid redundant calls to stop recording
                {
                    DBG("Stopping recording...");
                    audioRecorder->releaseResources();  // Stop the recorder
                    DBG("Recording stopped");
                }
            }
        }

        // Update the button text based on the state
        if (isOnStates[buttonIndex])
        {
            toggleButtons[buttonIndex]->setButtonText("ON");
            DBG("Button " + std::to_string(buttonIndex) + " Switch ON");
        }
        else
        {
            toggleButtons[buttonIndex]->setButtonText("OFF");
            DBG("Button " + std::to_string(buttonIndex) + " Switch OFF");
        }
    }
}

