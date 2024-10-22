/*
  ==============================================================================

    ToggleSlider.cpp
    Created: 22 Oct 2024 3:25:49pm
    Author:  ASUS

  ==============================================================================
*/

#include "ToggleSlider.h"

ToggleSlider::ToggleSlider()
    : juce::Button("ToggleSlider"), animationProgress(0.0f), isAnimating(false)
{
    setClickingTogglesState(true);  // Enable toggle behavior
}

ToggleSlider::~ToggleSlider()
{
    
}

void ToggleSlider::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    juce::Colour backgroundColour = getToggleState() ? juce::Colours::green : juce::Colours::red;
    juce::Colour interpolatedColour = backgroundColour.interpolatedWith(juce::Colours::blue, animationProgress);

    g.setColour(interpolatedColour);
    g.fillRoundedRectangle(bounds, 10.0f);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f));
    g.drawText("Custom Toggle", getLocalBounds(), juce::Justification::centred, true);
}

void ToggleSlider::clicked()
{
    startTimerHz(60);
    isAnimating = true;
}

void ToggleSlider::timerCallback()
{
    if (isAnimating)
    {
        animationProgress += 0.05f;

        if (animationProgress >= 1.0f)
        {
            animationProgress = 1.0f;
            isAnimating = false;
            stopTimer();
        }

        repaint();
    }
}
