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
    SimpleSamplerAudioProcessor& audioProcessor;
    const std::string name;
    std::string centerFrequencyAttachmentName;
    std::string QAttachmentName;
    std::string filterSelectorNameAttachmentName;
    std::string gainAttachmentName;
    juce::GroupComponent filterGroup;
    juce::Slider centerFrequencySlider, QSlider, gainSlider;
    juce::ComboBox filterSelector;
    juce::ToggleButton bypassButton;
    juce::Label centerFrequencyLabel, QLabel, filterSelectorLabel, bypassLabel, gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centerFrequencyAttachment, QAttachment, gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>  filterSelectorAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};