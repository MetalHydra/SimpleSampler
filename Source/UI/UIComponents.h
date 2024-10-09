/*
  ==============================================================================

    BaseComponents.h
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

class UIComponents : public juce::Component
{
public:
    UIComponents(SimpleSamplerAudioProcessor& p);
    ~UIComponents() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:    
    SimpleSamplerAudioProcessor& audioProcessor;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider, gainSlider;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel, gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment, gainAttachment;


    juce::ComboBox sampleSelector, filterSelector;
    juce::Label sampleSelectorLabel, filterSelectorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sampleSelectorAttachment, filterSelectorAttachment;

    juce::Slider roomSlider, dampSlider, wetSlider, widthSlider;
    juce::Label roomLabel, dampLabel, wetLabel, widthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomAttachment, dampAttachment, wetAttachment, widthAttachment;

    juce::Slider lowpassCutOffSlider, highpassCutOffSlider;
    juce::Label lowpassCutOffLabel, highpassCutOffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassCutOffAttachment, highpassCutOffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIComponents)
};