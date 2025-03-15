#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <JuceHeader.h>

class Equalizer : public juce::Component, public juce::Slider::Listener {
public:
    Equalizer();
    ~Equalizer() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    static constexpr int numBands = 10;  // Jumlah band equalizer
    juce::Slider sliders[numBands];      // Array slider
    juce::TextButton resetButton{ "Reset" };  // Tombol reset

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Equalizer)
};

#endif