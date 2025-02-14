#include "SpeechDSP.h"

SpeechDSP::SpeechDSP() 
{

}

SpeechDSP::~SpeechDSP() 
{

}

void SpeechDSP::prepare(double sampleRate, int samplesPerBlock)
{
    fftProcess.setSampleRate(sampleRate);
    juce::Logger::getCurrentLogger()->writeToLog("SpeechDSP prepared");
}

void SpeechDSP::process(juce::AudioBuffer<float>& buffer)
{
    /*if (!fftProcess.detectSpeech())
    {
        buffer.clear();
        return;
    }*/

    // Temporarily disabled

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= 1.0f;
        }
    }
}