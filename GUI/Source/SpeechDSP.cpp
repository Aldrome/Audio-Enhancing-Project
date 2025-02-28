#include "SpeechDSP.h"

SpeechDSP::SpeechDSP() 
{

}

SpeechDSP::~SpeechDSP() 
{

}

void SpeechDSP::prepare(double sampleRate, int samplesPerBlock)
{
    juce::Logger::getCurrentLogger()->writeToLog("SpeechDSP prepared");
}

void SpeechDSP::process(juce::AudioBuffer<float>& buffer)
{


    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] *= 1.0f;
        }
    }
}