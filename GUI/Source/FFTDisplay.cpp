#include "FFTDisplay.h"

FFTDisplay::FFTDisplay()
    : spectrogramImage(juce::Image::RGB, widthSize, fftSize / 2, true) // True = has an alpha channel
{
    spectrogramImage.clear(spectrogramImage.getBounds(), juce::Colours::black);
    startTimerHz(30);
}

FFTDisplay::~FFTDisplay()
{
    
}

void FFTDisplay::setFFTData(const std::array<float, 512>& newFFTData)
{
    writeIndex = (writeIndex + 1) % widthSize; // Circular buffer update

    for (size_t y = 0; y < fftSize / 2; ++y)
    {
        float magnitude = juce::jlimit(0.0f, 1.0f, newFFTData[y]); // Normalize
        juce::Colour color = juce::Colour::fromHSV(0.67f - magnitude * 0.5f, 1.0f, magnitude, 1.0f);
        spectrogramImage.setPixelAt(writeIndex, static_cast<int>(y), color);
    }
}

void FFTDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.drawImage(spectrogramImage, getLocalBounds().toFloat());
}

void FFTDisplay::timerCallback()
{
    repaint();
}

int FFTDisplay::getBinIndex(float frequency, float sampleRate) const
{
    return static_cast<int>((frequency / (sampleRate / 2)) * (fftSize / 2));
}
