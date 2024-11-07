//
// Created by sekiro on 01.11.24.
//
#include "FilterComponents.h"

FilterComponents::FilterComponents(SimpleSamplerAudioProcessor& p) : audioProcessor(p)
{
    centerFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    centerFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(centerFrequencySlider);
    centerFrequencyLabel.setText("Cutoff", juce::dontSendNotification);
    centerFrequencyLabel.setJustificationType(juce::Justification::centredTop);
    centerFrequencyLabel.attachToComponent(&centerFrequencySlider, false);
    centerFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "CUTOFF", centerFrequencySlider);

    QSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(QSlider);
    QLabel.setText("Q", juce::dontSendNotification);
    QLabel.setJustificationType(juce::Justification::centredTop);
    QLabel.attachToComponent(&QSlider, false);
    QAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "Q", QSlider);

    addAndMakeVisible(filterSelector);
    filterSelector.addItem("Lowpass", 1);
    filterSelector.addItem("Highpass", 2);
    filterSelector.addItem("Bandpass", 3);
    filterSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    filterSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), "FILTER", filterSelector);
}

FilterComponents::~FilterComponents()
{
}

void FilterComponents::paint(juce::Graphics& g)
{
}

void FilterComponents::resized()
{
    centerFrequencySlider.setBoundsRelative(0, 0.1, 0.1f, 0.7f);
    QSlider.setBoundsRelative(0.06f, 0.1, 0.1f, 0.7f);
    filterSelector.setBoundsRelative(0.025, 0.8, 0.2, 0.06);
}