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

    widthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40,20);
    addAndMakeVisible(widthSlider);
    widthLabel.setText("width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centredTop);
    widthLabel.attachToComponent(&widthSlider, false);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WIDTH", widthSlider);

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

    addAndMakeVisible(sampleSelector);
    sampleSelector.addItem("Sample1", 1);
    sampleSelector.addItem("Sample2", 2);
    sampleSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), "SAMPLE", sampleSelector);

    addAndMakeVisible(filterSelector);
    filterSelector.addItem("Lowpass", 1);
    filterSelector.addItem("Highpass", 2);
    filterSelector.addItem("Bandpass", 3);
    filterSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getAPVTS(), "FILTER", filterSelector);
}

UIComponents::~UIComponents()
{
}

void UIComponents::paint(juce::Graphics& g)
{

    juce::ColourGradient gradient (juce::Colours::darkgrey, 400.0f, 0.0f,  juce::Colours::slategrey, 0.0f, 600.0f, false);
    g.setGradientFill(gradient);
    g.setOpacity(0.8f);

    g.fillAll();
}

void UIComponents::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);

    attackSlider.setBoundsRelative(0.0f, 0.5f, 0.1f, 0.2f);
    decaySlider.setBoundsRelative(0.06f, 0.5f, 0.1f, 0.2f);
    sustainSlider.setBoundsRelative(0.12f, 0.5f, 0.1f, 0.2f);
    releaseSlider.setBoundsRelative(0.18f, 0.5f, 0.1f, 0.2f);
    gainSlider.setBoundsRelative(0.24, 0.5f, 0.1f, 0.2f);

    roomSlider.setBoundsRelative(0.72f, 0.5f, 0.1f, 0.2f);
    dampSlider.setBoundsRelative(0.78f, 0.5f, 0.1f, 0.2f);
    wetSlider.setBoundsRelative(0.84f, 0.5f, 0.1f, 0.2f);
    widthSlider.setBoundsRelative(0.9f, 0.5f, 0.1f, 0.2f);

    lowpassCutOffSlider.setBoundsRelative(0, 0.1, 0.1f, 0.18f);
    highpassCutOffSlider.setBoundsRelative(0.06f, 0.1, 0.1f, 0.18f);

    sampleSelector.setBoundsRelative(0.45, 0.5, 0.15, 0.06);
    filterSelector.setBoundsRelative(0.025, 0.3, 0.13, 0.04);
}