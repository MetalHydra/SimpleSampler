//
// Created by sekiro on 31.10.24.
//

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

class FilterComponents : public juce::Component
{
public:
    FilterComponents(SimpleSamplerAudioProcessor& p);
    ~FilterComponents() override;
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    SimpleSamplerAudioProcessor& audioProcessor;
    juce::Slider lowpassCutOffSlider, highpassCutOffSlider;
    juce::Label lowpassCutOffLabel, highpassCutOffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassCutOffAttachment, highpassCutOffAttachment;

    juce::ComboBox filterSelector;
    juce::Label filterSelectorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>  filterSelectorAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponents)
};