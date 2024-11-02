#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    setSize(600, 600);

    const int numberOfButtons = 3;  // Number of buttons

    // Initialize states for buttons
    isOnStates.resize(numberOfButtons, false);  // Initialize all states to OFF (false)

    // Create the buttons
    for (int i = 0; i < numberOfButtons; ++i)
    {
        auto button = std::make_unique<juce::TextButton>();
        button->setButtonText("OFF");  // Initial state
        button->onClick = [this, i] { toggleButtonClicked(i); };  // Capture the button index
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
}

//==============================================================================
// Toggle button click handler
void MainComponent::toggleButtonClicked(int buttonIndex)
{
    if (buttonIndex >= 0 && buttonIndex < isOnStates.size()) // Check for valid index
    {
        isOnStates[buttonIndex] = !isOnStates[buttonIndex];  // Toggle the state

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
