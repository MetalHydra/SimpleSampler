/*
  ==============================================================================

    BaseComponents.cpp
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#include<JuceHeader.h>
#include "UIComponents.h"

UIComponents::UIComponents(SimpleSamplerAudioProcessor& p) : audioProcessor(p) , keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(attackSlider);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centredTop);
    attackLabel.attachToComponent(&attackSlider, false);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", attackSlider);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(decaySlider);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centredTop);
    decayLabel.attachToComponent(&decaySlider, false);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", decaySlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(sustainSlider);
    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setJustificationType(juce::Justification::centredTop);
    sustainLabel.attachToComponent(&sustainSlider, false);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(releaseSlider);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centredTop);
    releaseLabel.attachToComponent(&releaseSlider, false);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", releaseSlider);


    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(gainSlider);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centredTop);
    gainLabel.attachToComponent(&gainSlider, false);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "GAIN", gainSlider);

    addAndMakeVisible(sampleSelector);
    sampleSelector.addItem("Sample1", 1);
    sampleSelector.addItem("Sample2", 2);
    sampleSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), "SAMPLE", sampleSelector);

    roomSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    roomSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(roomSlider);
    roomLabel.setText("room", juce::dontSendNotification);
    roomLabel.setJustificationType(juce::Justification::centredTop);
    roomLabel.attachToComponent(&roomSlider, false);
    roomAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ROOM", roomSlider);

    dampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dampSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(dampSlider);
    dampLabel.setText("damp", juce::dontSendNotification);
    dampLabel.setJustificationType(juce::Justification::centredTop);
    dampLabel.attachToComponent(&dampSlider, false);
    dampAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DAMP", dampSlider);

    wetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(wetSlider);
    wetLabel.setText("wet", juce::dontSendNotification);
    wetLabel.setJustificationType(juce::Justification::centredTop);
    wetLabel.attachToComponent(&wetSlider, false);
    wetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WET", wetSlider);

    drySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    drySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(drySlider);
    dryLabel.setText("dry", juce::dontSendNotification);
    dryLabel.setJustificationType(juce::Justification::centredTop);
    dryLabel.attachToComponent(&drySlider, false);
    dryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DRY", drySlider);

    widthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(widthSlider);
    widthLabel.setText("width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centredTop);
    widthLabel.attachToComponent(&widthSlider, false);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WIDTH", widthSlider);

    freezeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    freezeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(freezeSlider);
    freezeLabel.setText("freeze", juce::dontSendNotification);
    freezeLabel.setJustificationType(juce::Justification::centredTop);
    freezeLabel.attachToComponent(&freezeSlider, false);
    freezeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "FREEZE", freezeSlider);
}

UIComponents::~UIComponents()
{
}

void UIComponents::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void UIComponents::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);

    attackSlider.setBoundsRelative(0.0f, 0.5f, 0.1f, 0.2f);
    decaySlider.setBoundsRelative(0.06f, 0.5f, 0.1f, 0.2f);
    sustainSlider.setBoundsRelative(0.12f, 0.5f, 0.1f, 0.2f);
    releaseSlider.setBoundsRelative(0.18f, 0.5f, 0.1f, 0.2f);
    gainSlider.setBoundsRelative(0.24, 0.5f, 0.1f, 0.2f);

    sampleSelector.setBounds(450, 250, 200, 20);


    roomSlider.setBoundsRelative(0.60f, 0.5f, 0.1f, 0.2f);
    dampSlider.setBoundsRelative(0.66f, 0.5f, 0.1f, 0.2f);
    wetSlider.setBoundsRelative(0.72f, 0.5f, 0.1f, 0.2f);
    drySlider.setBoundsRelative(0.78f, 0.5f, 0.1f, 0.2f);
    widthSlider.setBoundsRelative(0.84f, 0.5f, 0.1f, 0.2f);
    freezeSlider.setBoundsRelative(0.9f, 0.5f, 0.1f, 0.2f);

}