#include "MainComponent.h"

//==============================================================================
// 構造函數
MainComponent::MainComponent()
{
    setSize(400, 300);
    formatManager.registerBasicFormats();
    deviceManager.initialise(2, 2, nullptr, true); // 初始化音訊裝置（2 輸入，2 輸出）
    deviceManager.addAudioCallback(this);
}

// 解構函數
MainComponent::~MainComponent()
{
    deviceManager.removeAudioCallback(this);
}

// 處理音訊流
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

// 回音/回授消除
void MainComponent::applyEchoCancellation(juce::AudioBuffer<float>& buffer)
{
    float alpha = 0.8f; // 低通濾波器係數
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int i = 1; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = alpha * channelData[i] + (1.0f - alpha) * channelData[i - 1];
        }
    }
}

// 降噪處理
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

// 動態範圍壓縮（DRC）
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

// 語音增強（Speech Enhancement）
void MainComponent::applySpeechEnhancement(juce::AudioBuffer<float>& buffer)
{
    juce::IIRFilter filter;
    filter.coefficients = juce::IIRCoefficients::makePeakFilter(44100, 3000.0, 1.0, 1.5);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        filter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
    }
}

// GUI 繪製
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

// 視窗大小變更
void MainComponent::resized()
{
}
