//
// Created by sekiro on 06.11.24.
//

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

class Instruments : public juce::Component {
public:
     Instruments(SimpleSamplerAudioProcessor& processor);
     ~Instruments() override;

     void paint (juce::Graphics& g) override;
     void resized() override;

private:
    SimpleSamplerAudioProcessor& processor;
    juce::ComboBox sampleSelector;
    juce::Label sampleSelectorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sampleSelectorAttachment;
};