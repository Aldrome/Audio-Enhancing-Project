#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    setSize(600, 600);

    // Initialize the slider
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    volumeSlider.setRange(0.0, 1.0, 0.01);  // Volume range from 0 to 1
    volumeSlider.setValue(0.5);             // Default value
    addAndMakeVisible(volumeSlider);

    // Pass volumeSlider to the AudioRecorder
    audioRecorder = std::make_unique<AudioRecorder>(volumeSlider);

    const int numberOfButtons = 3;  // Number of buttons

    // Initialize states for buttons
    isOnStates.resize(numberOfButtons, false);  // Initialize all states to OFF (false)

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
    // No need to explicitly delete equalizer, unique_ptr handles it.
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
    const int buttonAreaTop = 150; // Start position for the buttons
    const int buttonAreaBottomPadding = 30; // Space below the buttons for the equalizer

    // Layout the volume slider at the top
    volumeSlider.setBounds(
        20,
        70,
        getWidth() - 40,
        40
    );

    // Layout the buttons below the slider
    for (size_t i = 0; i < toggleButtons.size(); ++i)
    {
        toggleButtons[i]->setBounds(
            20,
            buttonAreaTop + (buttonHeight + buttonSpacing) * i,
            buttonWidth,
            buttonHeight
        );
    }

    // Calculate the area occupied by the buttons
    const int totalButtonHeight = (buttonHeight + buttonSpacing) * toggleButtons.size();
    const int equalizerTop = buttonAreaTop + totalButtonHeight + buttonAreaBottomPadding;

    // Set the bounds for the equalizer component
    if (equalizer) // Ensure the equalizer is valid
    {
        equalizer->setBounds(
            20,
            equalizerTop,
            getWidth() - 40,
            getHeight() - equalizerTop - 40
        );
    }

}

//==============================================================================
// Toggle button click handler
void MainComponent::toggleButtonClicked(int buttonIndex)
{
    if (buttonIndex >= 0 && buttonIndex < isOnStates.size()) // Check for valid index
    {
        DBG("Toggling button " + std::to_string(buttonIndex)); // Debugging output

        isOnStates[buttonIndex] = !isOnStates[buttonIndex];  // Toggle the state

        // If this is the button that controls audio recording, start/stop recording
        if (buttonIndex == 2)  // Assuming buttonIndex 2 controls recording
        {
            if (isOnStates[buttonIndex])
            {
                audioRecorder->prepareToPlay(512, 44100.0);  // Call necessary methods
                DBG("Recording started");
            }
            else
            {
                audioRecorder->releaseResources();
                DBG("Recording stopped");
            }
        }

        // Update button text based on the state
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
