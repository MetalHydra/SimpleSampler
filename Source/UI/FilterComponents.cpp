//
// Created by sekiro on 01.11.24.
//
#include "FilterComponents.h"

FilterComponents::FilterComponents(SimpleSamplerAudioProcessor& p) : audioProcessor(p)
{
    lowpassCutOffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    lowpassCutOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(lowpassCutOffSlider);
    lowpassCutOffLabel.setText("lowpass", juce::dontSendNotification);
    lowpassCutOffLabel.setJustificationType(juce::Justification::centredTop);
    lowpassCutOffLabel.attachToComponent(&lowpassCutOffSlider, false);
    lowpassCutOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "LOWPASS", lowpassCutOffSlider);

    highpassCutOffSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    highpassCutOffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(highpassCutOffSlider);
    highpassCutOffLabel.setText("highpass", juce::dontSendNotification);
    highpassCutOffLabel.setJustificationType(juce::Justification::centredTop);
    highpassCutOffLabel.attachToComponent(&highpassCutOffSlider, false);
    highpassCutOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "HIGHPASS", highpassCutOffSlider);

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
    lowpassCutOffSlider.setBoundsRelative(0, 0.1, 0.1f, 0.18f);
    highpassCutOffSlider.setBoundsRelative(0.06f, 0.1, 0.1f, 0.18f);
    filterSelector.setBoundsRelative(0.025, 0.3, 0.13, 0.04);
}