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

    int height = fftSize / 2;
    for (size_t y = 0; y < height; ++y)
    {
        float magnitude = juce::jlimit(0.0f, 1.0f, newFFTData[y]);
        juce::Colour color = getSpectrogramColour(magnitude);
        spectrogramImage.setPixelAt(writeIndex, height - 1 - static_cast<int>(y), color); // Flip vertically
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

juce::Colour FFTDisplay::getSpectrogramColour(float magnitude)
{
    magnitude = juce::jlimit(0.0f, 1.0f, magnitude);

    // Map magnitude (0.0 to 1.0) to a warm-to-hot color gradient
    if (magnitude < 0.2f)
        return juce::Colour::fromRGB(0, 0, static_cast<juce::uint8>(magnitude * 255 * 5)); // Black → Blue
    else if (magnitude < 0.4f)
        return juce::Colour::fromRGB(static_cast<juce::uint8>((magnitude - 0.2f) * 255 * 5), 0, 255); // Blue → Purple
    else if (magnitude < 0.6f)
        return juce::Colour::fromRGB(255, 0, static_cast<juce::uint8>((0.6f - magnitude) * 255 * 5)); // Purple → Red
    else if (magnitude < 0.8f)
        return juce::Colour::fromRGB(255, static_cast<juce::uint8>((magnitude - 0.6f) * 255 * 5), 0); // Red → Orange
    else
        return juce::Colour::fromRGB(255, 255, static_cast<juce::uint8>((magnitude - 0.8f) * 255 * 5)); // Orange → Yellow → White
}
