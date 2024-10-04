/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/UIComponents.h"
#include "UI/InstrumentUi.h"

//==============================================================================
/**
*/
class SimpleSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleSamplerAudioProcessorEditor (SimpleSamplerAudioProcessor&);
    ~SimpleSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //SimpleSamplerAudioProcessor& audioProcessor;
    UIComponents mainComponents;
    InstrumentUi instrumentUi;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSamplerAudioProcessorEditor)
};
