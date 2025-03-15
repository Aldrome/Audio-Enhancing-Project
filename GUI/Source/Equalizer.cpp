#include "Equalizer.h"

Equalizer::Equalizer() {
    // Inisialisasi slider
    for (int i = 0; i < numBands; i++) {
        sliders[i].setSliderStyle(juce::Slider::LinearVertical);
        sliders[i].setRange(0.0, 100.0, 1.0);
        sliders[i].setValue(50.0);  // Default posisi tengah
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
        sliders[i].addListener(this);
        addAndMakeVisible(sliders[i]);
    }

    // Konfigurasi tombol reset
    resetButton.onClick = [this]() {
        for (int i = 0; i < numBands; i++) {
            sliders[i].setValue(50.0);
        }
        };
    addAndMakeVisible(resetButton);
}

Equalizer::~Equalizer() {}

void Equalizer::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("Equalizer", getLocalBounds(), juce::Justification::centredTop, true);
}

void Equalizer::resized() {
    int sliderWidth = getWidth() / (numBands + 1);
    int sliderHeight = getHeight() - 50;

    for (int i = 0; i < numBands; i++) {
        sliders[i].setBounds(i * sliderWidth + 10, 30, sliderWidth - 10, sliderHeight - 50);
    }

    resetButton.setBounds(getWidth() / 2 - 50, getHeight() - 40, 100, 30);
}

void Equalizer::sliderValueChanged(juce::Slider* slider) {
    // Log perubahan nilai slider
    DBG("Slider " << (slider - sliders) << " value: " << slider->getValue());
}