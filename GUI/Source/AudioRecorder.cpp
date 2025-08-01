#include "AudioRecorder.h"

AudioRecorder::AudioRecorder()
    : isRecording(false),
    fftBuffer(fftSize, 0.0f),
    fifoBuffer(fftSize, 0.0f)
{
    setAudioChannels(2, 2);
}

AudioRecorder::~AudioRecorder()
{
    shutdownAudio();
    DBG("AudioRecorder destroyed");
    fftBuffer.clear();
    fifoBuffer.clear();
}

void AudioRecorder::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    isRecording = true;
    fifoIndex = 0;
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;

    updateFilters();
    highPassFilter.prepare(spec);
    lowPassFilter.prepare(spec);
    highShelfFilter.prepare(spec);
    midBoostFilter.prepare(spec);
    // highBandExpander.prepare(spec);

    speechDetector.prepare(sampleRate, fftSize);
    energyDetector.prepare(sampleRate);
    speechBalancer.prepare(spec);
}


void AudioRecorder::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!isRecording)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    // Feed raw input into FFT & energy detector
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        const float* inputData = bufferToFill.buffer->getReadPointer(channel);
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            float rawSample = inputData[sample];
            pushNextSampleIntoFifo(rawSample);          // FFT
            energyDetector.processSample(rawSample);    // Energy detection
        }
    }

    // After detection is updated, we can now safely check detection state
    bool speechDetected = speechDetector.isSpeechDetected();
    bool energyDetected = energyDetector.isEnergyDetected();

    // Processed path
    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    if (filterEnabled)
    {
        highPassFilter.process(context);
        lowPassFilter.process(context);
        highShelfFilter.process(context);
        midBoostFilter.process(context);
    }

    if (speechBalancerEnabled && (speechDetected && energyDetected))
    {
        speechBalancer.process(block);
    }

    // Volume control (after processing)
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            buffer[sample] *= volume.load();
        }
    }

    const float* filteredData = bufferToFill.buffer->getReadPointer(0); // just channel 0
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        pushNextSampleIntoFifo(filteredData[sample]);
    }

}




void AudioRecorder::releaseResources()
{
    isRecording = false;
}

void AudioRecorder::setFilterEnabled(bool enabled)
{
    filterEnabled = enabled;
}


void AudioRecorder::setSpeechBalancerEnabled(bool enabled)
{
    speechBalancerEnabled = enabled;
    speechBalancer.setEnabled(enabled);
}


void AudioRecorder::pushNextSampleIntoFifo(float sample)
{
    fifoBuffer[fifoIndex++] = sample;

    if (fifoIndex == fftSize)
    {
        std::copy(fifoBuffer.begin(), fifoBuffer.end(), fftBuffer.begin());
        fifoIndex = 0;
        processFFT();
    }
}

void AudioRecorder::processFFT()
{
    windowFunction.multiplyWithWindowingTable(fftBuffer.data(), fftSize);
    fft.performRealOnlyForwardTransform(fftBuffer.data());

    std::array<float, 512> fftArray;
    std::copy(fftBuffer.begin(), fftBuffer.begin() + 512, fftArray.begin());

    speechDetector.processFrame(fftArray);

    if (onFFTDataReady)
        onFFTDataReady(fftArray);
}

void AudioRecorder::setVolume(float newVolume)
{
    volume.store(juce::Decibels::decibelsToGain(newVolume));
}

void AudioRecorder::updateFilters()
{
    // High-pass filter (removes frequencies below 100 Hz)
    auto highPassCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(currentSampleRate, 100.0f);
    *highPassFilter.state = *highPassCoefficients;

    // Low-pass filter (removes frequencies above 10 kHz)
    auto lowPassCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, 6000.0f);
    *lowPassFilter.state = *lowPassCoefficients;

    // High-shelf EQ: Reduce boost & make it smoother
    auto highShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 4000.0f, 0.9f, 1.2f);
    *highShelfFilter.state = *highShelfCoefficients;

    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 1500.0f, 0.9f, 1.2f);
    *midBoostFilter.state = *peakCoefficients;

    //// Multi-band expander: Keep clarity but prevent excess noise boosting
    //highBandExpander.setThreshold(-33.0f);  // Slightly higher to avoid boosting noise
    //highBandExpander.setRatio(1.3f);        // Less aggressive expansion
    //highBandExpander.setAttack(10.0f);      // Faster response for clarity
    //highBandExpander.setRelease(120.0f);    // Reduce pumping and artifacts
}
