//
// Created by sekiro on 01.11.24.
//
#include "FilterComponent.h"

FilterComponent::FilterComponent(SimpleSamplerAudioProcessor& p, const std::string name) : audioProcessor(p), name(name)
{

    addAndMakeVisible(filterGroup);
    centerFrequencyAttachmentName = "Cutoff" + name;
    QAttachmentName = "Q" + name;
    filterSelectorNameAttachmentName = "Filter" + name;
    gainAttachmentName = "Gain" + name;

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

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.attachToComponent(&gainSlider, false);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), gainAttachmentName, gainSlider);

    addAndMakeVisible(filterSelector);
    filterSelector.addItem("Lowpass", 1);
    filterSelector.addItem("Highpass", 2);
    filterSelector.addItem("Bandpass", 3);
    filterSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    filterSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), filterSelectorNameAttachmentName, filterSelector);

    addAndMakeVisible(bypassButton);
    bypassLabel.setText("Bypass", juce::dontSendNotification);
    bypassLabel.attachToComponent(&bypassButton, true);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint(juce::Graphics& g)
{
}

void FilterComponent::resized()
{
    filterGroup.setText(name);
    filterGroup.setTextLabelPosition(juce::Justification::centredTop);
    filterGroup.setBounds(0,0,getWidth(),getHeight());
    filterSelector.setBoundsRelative(0.05f, 0.1f, 0.9f, 0.15f);
    centerFrequencySlider.setBoundsRelative(0.0f, 0.45f, 0.4f, 0.4f);
    centerFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,30);
    QSlider.setBoundsRelative(0.6f, 0.45f, 0.4f, 0.4f);
    QSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,30);
    gainSlider.setBoundsRelative(0.35f, 0.4f, 0.3f, 0.3f);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,30);
    bypassButton.setBoundsRelative(0.4f, 0.85f, 0.4f, 0.15f);

}