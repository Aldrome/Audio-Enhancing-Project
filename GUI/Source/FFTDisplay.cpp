#include <JuceHeader.h>
#include "FFTDisplay.h"

//==============================================================================
FFTDisplay::FFTDisplay()
{
	fftData.fill(0);
	startTimerHz(30); // Refresh UI at 30 FPS
}

FFTDisplay::~FFTDisplay()
{

}

void FFTDisplay::setFFTData(const std::array<float, 1024>& newFFTData)
{
	fftData = newFFTData;
	repaint();
}

void FFTDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    juce::Path path;
    float width = getWidth();
    float height = getHeight();
    float binWidth = width / (fftSize / 2.0f);

    for (int i = 0; i < fftSize / 2; ++i)
    {
        float magnitude = juce::jmap(juce::Decibels::gainToDecibels(fftData[i]), -100.0f, 0.0f, height, 0.0f);
        if (i == 0)
            path.startNewSubPath(i * binWidth, magnitude);
        else
            path.lineTo(i * binWidth, magnitude);
    }

    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void FFTDisplay::timerCallback()
{
    repaint();
}