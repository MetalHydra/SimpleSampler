/*
  ==============================================================================

    BaseComponents.cpp
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#include<JuceHeader.h>
#include "BaseComponents.h"

CustomComponent::CustomComponent(SimpleSamplerAudioProcessor& p) : audioProcessor(p) , keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(attackSlider);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredTop);
    attackLabel.attachToComponent(&attackSlider, false);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", attackSlider);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredTop);
    decayLabel.attachToComponent(&decaySlider, false);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", decaySlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(sustainSlider);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredTop);
    sustainLabel.attachToComponent(&sustainSlider, false);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(releaseSlider);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredTop);
    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", releaseSlider);
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void CustomComponent::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);

    attackSlider.setBoundsRelative(0.1f, 0.5f, 0.1f, 0.3f);
    decaySlider.setBoundsRelative(0.3f, 0.5f, 0.1f, 0.3f);
    sustainSlider.setBoundsRelative(0.5f, 0.5f, 0.1f, 0.3f);
    releaseSlider.setBoundsRelative(0.7f, 0.5f, 0.1f, 0.3f);
}