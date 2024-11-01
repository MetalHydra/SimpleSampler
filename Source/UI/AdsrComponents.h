//
// Created by dennis on 01.11.24.
//

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class AdsrComponents : public juce::Component {

    public:
          AdsrComponents(SimpleSamplerAudioProcessor& p);
          ~AdsrComponents() override;
          void paint(juce::Graphics &g) override;
          void resized() override;

    private:
          SimpleSamplerAudioProcessor& audioProcessor;
          juce::Slider gainSlider,  attackSlider, decaySlider, sustainSlider, releaseSlider;
          juce::Label gainLabel, attackLabel, decayLabel, sustainLabel, releaseLabel;
          std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment, attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;

          JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponents)
};