//
// Created by sekiro on 31.10.24.
//

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(SimpleSamplerAudioProcessor& p, std::string name);
    ~FilterComponent() override;
    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    std::string name = "Filter";
    std::string centerFrequencyAttachmentName = "CUTOFF";
    std::string QAttachmentName = "Q";
    std::string filterSelectorNameAttachmentName = "FILTER";
    juce::GroupComponent filterGroup;
    SimpleSamplerAudioProcessor& audioProcessor;
    juce::Slider centerFrequencySlider, QSlider;
    juce::ComboBox filterSelector;
    juce::Label centerFrequencyLabel, QLabel, filterSelectorLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centerFrequencyAttachment, QAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>  filterSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};