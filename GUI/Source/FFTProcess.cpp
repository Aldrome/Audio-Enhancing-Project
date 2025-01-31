#include <JuceHeader.h>
#include "FFTProcess.h"

//==============================================================================
FFTProcess::FFTProcess()
{
    fifoIndex = 0;
    fftData.fill(0);
}

FFTProcess::~FFTProcess()
{

}

void FFTProcess::processFFT(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            pushNextSampleIntoFifo(channelData[sample]);
        }
    }
}

void FFTProcess::pushNextSampleIntoFifo(float sample)
{
    if (fifoIndex == fftSize)
    {
        std::copy(fifo.begin(), fifo.end(), fftData.begin());
        window.multiplyWithWindowingTable(fftData.data(), fftSize);
        forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());
        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}