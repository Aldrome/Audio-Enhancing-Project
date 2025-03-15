#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    setSize(600, 600);

    // Initialize unique pointers
    audioRecorder = std::make_unique<AudioRecorder>();
    fftDisplay = std::make_unique<FFTDisplay>();
    equalizer = std::make_unique<Equalizer>();
    volumeSlider = std::make_unique<VolumeSlider>();

    // Set up the volume slider callback
    volumeSlider->onValueChange = [&](float newVolume) {
        audioRecorder->setVolume(newVolume);
    };

    audioRecorder->onFFTDataReady = [this](const std::array<float, 512>& fftData)
    {
        handleFFTData(fftData);
    };

    addAndMakeVisible(volumeSlider.get());

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

    // Add the Equalizer
    addAndMakeVisible(equalizer.get());

    // Add the FFT Display
    addAndMakeVisible(fftDisplay.get());

    resized();  // Call resized to set up positions
}

MainComponent::~MainComponent() = default;

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
    // Placement values for ease of access
    const int buttonWidth = 100;
    const int buttonHeight = 50;
    const int buttonSpacing = 20;
    const int buttonAreaTop = 150; // Start position for the buttons
    const int buttonAreaBottomPadding = 30; // Space below the buttons for the equalizer

    // Calculate the area occupied by the buttons
    const int totalButtonHeight = (buttonHeight + buttonSpacing) * toggleButtons.size();
    const int equalizerTop = buttonAreaTop + totalButtonHeight + buttonAreaBottomPadding;

    // Volume slider placement
    if (volumeSlider)
    {
        volumeSlider->setBounds(
            20,
            70,
            getWidth() - 40,
            40
        );
    }

    // Buttons placement
    for (size_t i = 0; i < toggleButtons.size(); ++i)
    {
        toggleButtons[i]->setBounds(
            20,
            buttonAreaTop + (buttonHeight + buttonSpacing) * i,
            buttonWidth,
            buttonHeight
        );
    }

    // Equalizer placement
    if (equalizer)
    {
        equalizer->setBounds(
            20,
            equalizerTop,
            getWidth() - 40,
            getHeight() - equalizerTop - 40
        );
    }

    if (fftDisplay)
    {
        fftDisplay->setBounds(
            20, 
            400, 
            getWidth() - 40, 
            200
        );
    };
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
                audioRecorder->prepareToPlay(256, 24000.0);  // Call necessary methods
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

void MainComponent::handleFFTData(const std::array<float, 512>& fftData)
{
    juce::MessageManager::callAsync([this, fftData]()
    {
        fftDisplay->setFFTData(fftData);
    });
}

