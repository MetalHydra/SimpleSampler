//
// Created by sekiro on 06.11.24.
//

#include "Instruments.h"

Instruments::Instruments(SimpleSamplerAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(sampleSelector);
    sampleSelector.addItem("Sample1", 1);
    sampleSelector.addItem("Sample2", 2);
    sampleSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    sampleSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.getAPVTS(), "SAMPLE", sampleSelector);
}

Instruments::~Instruments()
{
}

void Instruments::paint(juce::Graphics& g)
{
}

void Instruments::resized()
{
    sampleSelector.setBoundsRelative(0.45, 0.5, 0.15, 0.06);
}
