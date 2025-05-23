#include "SpeechBalancer.h"

SpeechBalancer::SpeechBalancer()
{
    // Noise gate setup (acts as downward expander)
    noiseGate.setThreshold(-30.0f);  // dB, signal below this will be suppressed
    noiseGate.setRatio(100.0f);      // High ratio to sharply cut low levels
    noiseGate.setAttack(10.0f);      // Fast attack
    noiseGate.setRelease(200.0f);    // Smooth release

    // Compressor setup
    compressor.setThreshold(-20.0f);  // dB
    compressor.setRatio(3.0f);        // Moderate compression
    compressor.setAttack(10.0f);      // Fast enough for speech
    compressor.setRelease(150.0f);    // Avoid pumping

    enabled = true;  // default enabled
}

void SpeechBalancer::prepare(const juce::dsp::ProcessSpec& spec)
{
    noiseGate.prepare(spec);
    compressor.prepare(spec);
    agc.prepare(spec);

    prepared = true;
}

void SpeechBalancer::process(juce::dsp::AudioBlock<float>& block)
{
    if (!prepared || !enabled)   // <-- Check if enabled
        return;

    DBG("SpeechBalancer: Processing block");
    juce::dsp::ProcessContextReplacing<float> context(block);
    noiseGate.process(context);
    compressor.process(context);
    agc.process(block);
}

void SpeechBalancer::setNoiseGateThreshold(float dB)
{
    noiseGate.setThreshold(dB);
}

void SpeechBalancer::setCompressorThreshold(float dB)
{
    compressor.setThreshold(dB);
}

void SpeechBalancer::setCompressorRatio(float ratio)
{
    compressor.setRatio(ratio);
}

void SpeechBalancer::setAttackRelease(float attackMs, float releaseMs)
{
    noiseGate.setAttack(attackMs);
    noiseGate.setRelease(releaseMs);
    compressor.setAttack(attackMs);
    compressor.setRelease(releaseMs);
}

void SpeechBalancer::setEnabled(bool shouldEnable)
{
    enabled = shouldEnable;
    agc.setEnabled(shouldEnable);
}

bool SpeechBalancer::isEnabled() const noexcept
{
    return enabled;
}
