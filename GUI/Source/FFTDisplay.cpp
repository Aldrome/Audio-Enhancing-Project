#include "FFTDisplay.h"

FFTDisplay::FFTDisplay()
    : spectrogramImage(juce::Image::RGB, widthSize, fftSize / 2, true)
{
    spectrogramImage.clear(spectrogramImage.getBounds(), juce::Colours::black);
    fftData.fill(0.0f);
    startTimerHz(30); // 30 FPS
}

FFTDisplay::~FFTDisplay()
{
    DBG("FFTDisplay destroyed");
}

void FFTDisplay::setFFTData(const std::array<float, 512>& newFFTData)
{
    fftData = newFFTData;

    // Write to spectrogram image
    writeIndex = (writeIndex + 1) % widthSize;
    for (int y = 0; y < fftSize / 2; ++y)
    {
        float magnitude = juce::jlimit(0.0f, 1.0f, newFFTData[y]);
        juce::Colour color = juce::Colour::fromHSV(0.66f - magnitude * 0.5f, 1.0f, magnitude, 1.0f);
        spectrogramImage.setPixelAt(writeIndex, y, color);
    }
}

void FFTDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    drawSpectrogram(g);
    drawSpectrum(g);
    drawGrid(g);
}

void FFTDisplay::drawSpectrogram(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.drawImage(spectrogramImage, bounds);
}

void FFTDisplay::drawSpectrum(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    juce::Path spectrumPath;

    float width = static_cast<float>(getWidth());
    float height = static_cast<float>(getHeight());
    float binWidth = width / (fftSize / 2.0f);

    for (int i = 0; i < fftSize / 2; ++i)
    {
        float dB = juce::Decibels::gainToDecibels(fftData[i], -100.0f);
        float magnitude = juce::jmap(dB, -100.0f, 0.0f, height, 0.0f);

        if (i == 0)
            spectrumPath.startNewSubPath(0.0f, magnitude);
        else
            spectrumPath.lineTo(i * binWidth, magnitude);
    }

    g.strokePath(spectrumPath, juce::PathStrokeType(1.5f));
}

void FFTDisplay::drawGrid(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::grey.withAlpha(0.3f));

    // Horizontal lines
    for (int i = 1; i < 4; ++i)
    {
        float y = bounds.getHeight() * i / 4.0f;
        g.drawHorizontalLine(static_cast<int>(y), 0.0f, static_cast<float>(bounds.getWidth()));
    }

    // Vertical lines
    for (int i = 1; i < 8; ++i)
    {
        float x = bounds.getWidth() * i / 8.0f;
        g.drawVerticalLine(static_cast<int>(x), 0.0f, static_cast<float>(bounds.getHeight()));
    }
}

void FFTDisplay::timerCallback()
{
    repaint();
}

