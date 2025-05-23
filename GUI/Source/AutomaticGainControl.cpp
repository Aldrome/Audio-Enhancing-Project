#include "AutomaticGainControl.h"

AutomaticGainControl::AutomaticGainControl() = default;

void AutomaticGainControl::prepare(const juce::dsp::ProcessSpec& spec)
{
    juce::ignoreUnused(spec);
    currentGain = 1.0f;
    prepared = true;
}

void AutomaticGainControl::process(juce::dsp::AudioBlock<float>& block)
{
    if (!enabled || !prepared)
        return;

    float rms = computeRMS(block);

    if (rms > 0.0f)
    {
        float desiredGain = targetRMS / rms;

        // Smooth gain adjustment
        currentGain += (desiredGain - currentGain) * adjustmentSpeed;
    }

    block.multiplyBy(currentGain);
}

void AutomaticGainControl::setEnabled(bool shouldBeEnabled)
{
    enabled = shouldBeEnabled;
}

bool AutomaticGainControl::isEnabled() const
{
    return enabled;
}

float AutomaticGainControl::computeRMS(const juce::dsp::AudioBlock<float>& block)
{
    double sum = 0.0;
    size_t count = 0;

    for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* data = block.getChannelPointer(channel);
        for (size_t i = 0; i < block.getNumSamples(); ++i)
        {
            float sample = data[i];
            sum += sample * sample;
            ++count;
        }
    }

    return count > 0 ? std::sqrt(sum / count) : 0.0f;
}
