//
// Created by dennis on 01.11.24.
//

#include "ReverbComponent.h"


ReverbComponent::ReverbComponent(SimpleSamplerAudioProcessor& p) : audioProcessor(p)
{
    roomSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    roomSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(roomSlider);
    roomLabel.setText("room", juce::dontSendNotification);
    roomLabel.setJustificationType(juce::Justification::centredTop);
    roomLabel.attachToComponent(&roomSlider, false);
    roomAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ROOM", roomSlider);

    dampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dampSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(dampSlider);
    dampLabel.setText("damp", juce::dontSendNotification);
    dampLabel.setJustificationType(juce::Justification::centredTop);
    dampLabel.attachToComponent(&dampSlider, false);
    dampAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DAMP", dampSlider);

    wetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(wetSlider);
    wetLabel.setText("wet", juce::dontSendNotification);
    wetLabel.setJustificationType(juce::Justification::centredTop);
    wetLabel.attachToComponent(&wetSlider, false);
    wetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WET", wetSlider);

    widthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(widthSlider);
    widthLabel.setText("width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centredTop);
    widthLabel.attachToComponent(&widthSlider, false);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WIDTH", widthSlider);
}

ReverbComponent::~ReverbComponent()
{

}

void ReverbComponent::paint(juce::Graphics& g)
{

}

void ReverbComponent::resized()
{
    roomSlider.setBoundsRelative(0.0f, 0.1f, 0.1f, 0.8f);
    dampSlider.setBoundsRelative(0.06f, 0.1f, 0.1f, 0.8f);
    wetSlider.setBoundsRelative(0.12f, 0.1f, 0.1f, 0.8f);
    widthSlider.setBoundsRelative(0.18f, 0.1f, 0.1f, 0.8f);
}