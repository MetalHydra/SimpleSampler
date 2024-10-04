/*
  ==============================================================================

    BaseComponents.h
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "InstrumentUi.h"


class CustomComponent : public juce::Component
{
public:
    CustomComponent(SimpleSamplerAudioProcessor& p);
    ~CustomComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

private:    
    SimpleSamplerAudioProcessor& audioProcessor;
    juce::MidiKeyboardComponent keyboardComponent;
    InstrumentUi instrumentUI;
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider, gainSlider;
    juce::ToggleButton pmToggleButton;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel, pmLabel, gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment, gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pmAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomComponent)
};