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
    attackSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xff0033ff));
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (attackSlider);    
    
    attackLabel.setFont (10.0f);
    attackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
    attackLabel.setJustificationType (juce::Justification::centredTop);
    attackLabel.attachToComponent(&attackSlider, false);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", attackSlider);
    
    decaySlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xff0033ff));
    decaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (decaySlider);    
    
    decayLabel.setFont (10.0f);
    decayLabel.setText ("Decay", juce::NotificationType::dontSendNotification);
    decayLabel.setJustificationType (juce::Justification::centredTop);
    decayLabel.attachToComponent(&attackSlider, false);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", decaySlider);
    
    sustainSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xff0033ff));
    sustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (sustainSlider);    
    
    sustainLabel.setFont (10.0f);
    sustainLabel.setText ("Sustain", juce::NotificationType::dontSendNotification);
    sustainLabel.setJustificationType (juce::Justification::centredTop);
    sustainLabel.attachToComponent(&attackSlider, false);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", sustainSlider);
    
    releaseSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(0xff0033ff));
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible (releaseSlider);
    
    releaseLabel.setFont (10.0f);
    releaseLabel.setText ("Release", juce::NotificationType::dontSendNotification);
    releaseLabel.setJustificationType (juce::Justification::centredTop);
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
    attackSlider.setBoundsRelative(0.05f, 0.5f, 0.2f, 0.2);
    decaySlider.setBoundsRelative(0.15f, 0.5f, 0.2f, 0.2);
    sustainSlider.setBoundsRelative(0.25f, 0.5f, 0.2f, 0.2);
    releaseSlider.setBoundsRelative(0.35f, 0.5f, 0.2f, 0.2);
}