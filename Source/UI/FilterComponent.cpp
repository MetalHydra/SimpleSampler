//
// Created by sekiro on 01.11.24.
//
#include "FilterComponent.h"

FilterComponent::FilterComponent(SimpleSamplerAudioProcessor& p, const std::string name) : audioProcessor(p), name(name)
{

    addAndMakeVisible(filterGroup);
    centerFrequencyAttachmentName = "Cutoff" + name;
    //DBG("center frequency attachment name: " + centerFrequencyAttachmentName);
    QAttachmentName = "Q" + name;
    //DBG("Q attachment name: " + QAttachmentName);
    filterSelectorNameAttachmentName = "Filter" + name;
    addAndMakeVisible(centerFrequencySlider);
    centerFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    centerFrequencyLabel.setText("Cutoff", juce::dontSendNotification);
    centerFrequencyLabel.setJustificationType(juce::Justification::centredTop);
    centerFrequencyLabel.attachToComponent(&centerFrequencySlider, false);
    centerFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), centerFrequencyAttachmentName, centerFrequencySlider);
    addAndMakeVisible(QSlider);
    QSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
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
    addAndMakeVisible(centerFrequencyLabel);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint(juce::Graphics& g)
{
}

void FilterComponent::resized()
{
    //DBG("size of window" + std::to_string(getWidth()) + ", " + std::to_string(getHeight()));
    filterGroup.setText(name);
    filterGroup.setTextLabelPosition(juce::Justification::centredTop);
    filterGroup.setBounds(0,0,getWidth(),getHeight());
    filterSelector.setBoundsRelative(0.05f, 0.1f, 0.9f, 0.15f);
    centerFrequencySlider.setBoundsRelative(0.0f, 0.45f, 0.51f, 0.51f);
    centerFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,30);
    QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,30);
    QSlider.setBoundsRelative(0.5f, 0.45f, 0.51f, 0.51f);
    //filterGroup.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}