#include "FFTDisplay.h"

FFTDisplay::FFTDisplay()
{
    spectrogramData.fill({});
    startTimerHz(10); // Reduce repaint calls for better performance
}

FFTDisplay::~FFTDisplay()
{
    DBG("FFTDisplay destroyed");
}

void FFTDisplay::setFFTData(const std::array<float, 512>& newFFTData)
{
    writeIndex = (writeIndex + 1) % widthSize; // Circular buffer update

    for (size_t i = 0; i < fftSize / 2; ++i)
    {
        spectrogramData[writeIndex][i] = newFFTData[i];
    }

    if (!isTimerRunning()) // Only repaint if necessary
        repaint();
}

void FFTDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    float width = getWidth();
    float height = getHeight();
    float columnWidth = width / static_cast<float>(widthSize);
    float rowHeight = height / static_cast<float>(fftSize / 2);

    for (size_t x = 0; x < widthSize; ++x)
    {
        int index = (writeIndex + x) % widthSize;
        for (size_t y = 0; y < fftSize / 2; ++y)
        {
            float magnitude = spectrogramData[index][y];
            float yPos = height - (y * rowHeight);

            g.setColour(juce::Colour::fromHSV(0.67f - magnitude * 0.5f, 1.0f, magnitude, 1.0f));
            g.fillRect(x * columnWidth, yPos, columnWidth, rowHeight);
        }
    }
}

void FFTDisplay::timerCallback()
{
    repaint();
}

int FFTDisplay::getBinIndex(float frequency, float sampleRate) const
{
    return static_cast<int>((frequency / (sampleRate / 2)) * (fftSize / 2));
}
