/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "BaseComponents.h"
#include "InstrumentUi.h"


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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    //SimpleSamplerAudioProcessor& audioProcessor;
    CustomComponent mainComponents;
    InstrumentUi instrumentUi;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSamplerAudioProcessorEditor)
};
