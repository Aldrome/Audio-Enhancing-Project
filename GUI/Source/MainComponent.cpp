#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    setSize(600, 600);

    const int numberOfButtons = 3; // Buttons
    const int numberOfEQslider = 10; // Equalizer

    // Initialize states
    isOnStates.resize(numberOfButtons, false); // Initialize all states to OFF (false)

    for (int i = 0; i < numberOfButtons; ++i)
    {
        // Create a new TextButton and store it in the vector
        auto button = std::make_unique<juce::TextButton>();
        button->setButtonText("OFF");  // Set initial text to OFF
        button->onClick = [this, i] { toggleButtonClicked(i); }; // Capture the button index
        addAndMakeVisible(button.get());  // Add the button to the component

        toggleButtons.push_back(std::move(button)); // Store the button in the vector
    }

    for (int i = 0; i < numberOfEQslider; ++i)
    {
        // Create a new vertical slider and store it in the vector
        auto slider = std::make_unique<juce::Slider>();
        slider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        slider->setRange(0.0, 1.0, 0.01); // Set the range from 0 to 1
        slider->setValue(0.5); // Set default value
        slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); // Hide the text box
        addAndMakeVisible(slider.get()); // Add the slider to the component

        verticalSliders.push_back(std::move(slider)); // Store the slider in the vector
    }

    resized();
}

MainComponent::~MainComponent()
{
}

//==============================================================================
// Paint method for drawing UI
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(50.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Audio Enhancement Project", getLocalBounds(), juce::Justification::centredTop, true);
}

//==============================================================================
// Resized method to lay out child components
void MainComponent::resized()
{
    // Set the bounds for each button
    const int buttonWidth = 100;
    const int buttonHeight = 50;
    const int buttonspacing = 20;

    for (size_t i = 0; i < toggleButtons.size(); ++i)
    {
        toggleButtons[i]->setBounds(20, 80 + (buttonHeight + buttonspacing) * i, buttonWidth, buttonHeight);  // Position the buttons
    }

    // Set the bounds for each slider
    const int EQsliderWidth = 50;
    const int EQsliderHeight = 200;
    const int EQspacing = 40;

    for (size_t i = 0; i < verticalSliders.size(); ++i)
    {
        verticalSliders[i]->setBounds(20 + EQsliderWidth + (EQspacing) * i, 80 + (buttonHeight + buttonspacing) * toggleButtons.size(), EQsliderWidth, EQsliderHeight);  // Position the sliders
    }
}

//==============================================================================
// Handle Toggle Button click
void MainComponent::toggleButtonClicked(int buttonIndex)
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
