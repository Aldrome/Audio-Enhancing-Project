#include "MainComponent.h"

//==============================================================================
// �c�y���
MainComponent::MainComponent()
{
    setSize(400, 300);
    formatManager.registerBasicFormats();
    deviceManager.initialise(2, 2, nullptr, true); // ��l�ƭ��T�˸m�]2 ��J�A2 ��X�^
    deviceManager.addAudioCallback(this);
}

// �Ѻc���
MainComponent::~MainComponent()
{
    deviceManager.removeAudioCallback(this);
}

// �B�z���T�y
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* device = deviceManager.getCurrentAudioDevice();
    if (device == nullptr) return;

    auto numInputChannels = device->getActiveInputChannels().countNumberOfSetBits();
    auto numOutputChannels = device->getActiveOutputChannels().countNumberOfSetBits();

    for (int channel = 0; channel < numOutputChannels; ++channel)
    {
        if (channel < numInputChannels)
        {
            bufferToFill.buffer->copyFrom(channel, 0, *bufferToFill.buffer, channel, 0, bufferToFill.numSamples);
        }
        else
        {
            bufferToFill.buffer->clear(channel, 0, bufferToFill.numSamples);
        }
    }

    applyEchoCancellation(*bufferToFill.buffer);
    applyNoiseReduction(*bufferToFill.buffer);
    applyDynamicRangeCompression(*bufferToFill.buffer);
    applySpeechEnhancement(*bufferToFill.buffer);
}

// �^��/�^�®���
void MainComponent::applyEchoCancellation(juce::AudioBuffer<float>& buffer)
{
    float alpha = 0.8f; // �C�q�o�i���Y��
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 1; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = alpha * channelData[i] + (1.0f - alpha) * channelData[i - 1];
        }
    }
}

// �����B�z
void MainComponent::applyNoiseReduction(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 1; i < buffer.getNumSamples() - 1; ++i)
        {
            channelData[i] = (channelData[i - 1] + channelData[i] + channelData[i + 1]) / 3.0f;
        }
    }
}

// �ʺA�d�����Y�]DRC�^
void MainComponent::applyDynamicRangeCompression(juce::AudioBuffer<float>& buffer)
{
    float threshold = 0.1f;
    float ratio = 2.0f;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            if (channelData[i] > threshold)
                channelData[i] = threshold + (channelData[i] - threshold) / ratio;
        }
    }
}

// �y���W�j�]Speech Enhancement�^
void MainComponent::applySpeechEnhancement(juce::AudioBuffer<float>& buffer)
{
    juce::IIRFilter filter;
    filter.coefficients = juce::IIRCoefficients::makePeakFilter(44100, 3000.0, 1.0, 1.5);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        filter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
    }
}

// GUI ø�s
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

// �����j�p�ܧ�
void MainComponent::resized()
{
}
