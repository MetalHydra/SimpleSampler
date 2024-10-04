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

    addAndMakeVisible(pmToggleButton);
    pmLabel.setText("Palm muting", juce::dontSendNotification);
    pmLabel.setJustificationType(juce::Justification::centredTop);
    pmLabel.attachToComponent(&pmToggleButton, false);
    pmAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "PM", pmToggleButton);

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(gainSlider);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.attachToComponent(&gainSlider, false);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "GAIN", gainSlider);
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    instrumentUI.paint(g);
}

void CustomComponent::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);

    attackSlider.setBoundsRelative(0.04f, 0.5f, 0.1f, 0.2f);
    decaySlider.setBoundsRelative(0.12f, 0.5f, 0.1f, 0.2f);
    sustainSlider.setBoundsRelative(0.20f, 0.5f, 0.1f, 0.2f);
    releaseSlider.setBoundsRelative(0.28f, 0.5f, 0.1f, 0.2f);
    pmToggleButton.setBoundsRelative(0.5f, 0.5f, 0.08f, 0.08f);
    gainSlider.setBoundsRelative(0.36f, 0.5f, 0.1f, 0.2f);
}