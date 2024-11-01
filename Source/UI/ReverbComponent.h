//
// Created by dennis on 01.11.24.
//

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class ReverbComponent : public Component {
public:
    ReverbComponent(SimpleSamplerAudioProcessor& p);
    ~ReverbComponent() override;
    void paint(Graphics& g) override;
    void resized() override;

private:
    SimpleSamplerAudioProcessor& audioProcessor;
    juce::Slider roomSlider, dampSlider, wetSlider, widthSlider;
    juce::Label roomLabel, dampLabel, wetLabel, widthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomAttachment, dampAttachment, wetAttachment, widthAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};