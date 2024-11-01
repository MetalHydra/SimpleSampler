/*
  ==============================================================================

    BaseComponents.h
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FilterComponents.h"
#include "AdsrComponents.h"
#include "ReverbComponent.h"
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

    FilterComponents filterComponents;
    AdsrComponents adsrComponents;
    ReverbComponent reverbComponent;

    juce::TabbedComponent tabbedComponent;

    juce::ComboBox sampleSelector;
    juce::Label sampleSelectorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sampleSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIComponents)
};