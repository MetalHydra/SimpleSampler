//
// Created by sekiro on 01.11.24.
//
#include "FilterComponent.h"

FilterComponent::FilterComponent(SimpleSamplerAudioProcessor& p, std::string name) : audioProcessor(p), name(std::move(name))
{
    addAndMakeVisible(filterGroup);
    centerFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    centerFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100,60);
    addAndMakeVisible(centerFrequencySlider);
    centerFrequencyLabel.setText("Cutoff", juce::dontSendNotification);
    centerFrequencyLabel.setJustificationType(juce::Justification::centredTop);
    centerFrequencyLabel.attachToComponent(&centerFrequencySlider, false);
    centerFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), centerFrequencyAttachmentName, centerFrequencySlider);

    QSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100,60);
    addAndMakeVisible(QSlider);
    QLabel.setText("Q", juce::dontSendNotification);
    QLabel.setJustificationType(juce::Justification::centredTop);
    QLabel.attachToComponent(&QSlider, false);
    QAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), QAttachmentName, QSlider);

    addAndMakeVisible(filterSelector);
    filterSelector.addItem("Lowpass", 1);
    filterSelector.addItem("Highpass", 2);
    filterSelector.addItem("Bandpass", 3);
    filterSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    filterSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), filterSelectorNameAttachmentName, filterSelector);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint(juce::Graphics& g)
{
    DBG("size of window" + std::to_string(getWidth()) + ", " + std::to_string(getHeight()));
}

void FilterComponent::resized()
{
    filterGroup.setBounds(0, 0, getWidth(), getHeight());
    filterSelector.setBoundsRelative(0.1f, 0.1f, 0.8f, 0.1f);
    centerFrequencySlider.setBoundsRelative(0.1f, 0.35f, 0.8f, 0.2f);
    QSlider.setBoundsRelative(0.1f, 0.6f, 0.8f, 0.2f);
    //filterGroup.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}