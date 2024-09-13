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
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomComponent)
};