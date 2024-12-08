/*
  ==============================================================================

    BaseComponents.h
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FilterComponent.h"
#include "AdsrComponents.h"
#include "ReverbComponent.h"
#include "Instruments.h"
#include "../PluginProcessor.h"
#include "EQ.h"

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
        EQ eqComponent;
        AdsrComponents adsrComponents;
        ReverbComponent reverbComponent;
        Instruments instruments;
        juce::TabbedComponent tabbedComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIComponents)
};