/*
  ==============================================================================

    BaseComponents.cpp
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#include<JuceHeader.h>
#include "UIComponents.h"

UIComponents::UIComponents(SimpleSamplerAudioProcessor& p) : audioProcessor(p) , keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard),
                                                             filterComponents(p), adsrComponents(p), reverbComponent(p) , tabbedComponent(juce::TabbedButtonBar::TabsAtTop) {
    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(sampleSelector);
    sampleSelector.addItem("Sample1", 1);
    sampleSelector.addItem("Sample2", 2);
    sampleSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
    sampleSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            audioProcessor.getAPVTS(), "SAMPLE", sampleSelector);

    addAndMakeVisible(tabbedComponent);
    tabbedComponent.addTab("Filters", juce::Colours::transparentBlack, &filterComponents, false);
    tabbedComponent.addTab("ADSR", juce::Colours::transparentBlack, &adsrComponents, false);
    tabbedComponent.addTab("Reverb", juce::Colours::transparentBlack, &reverbComponent, false);

}
UIComponents::~UIComponents()
{

}

void UIComponents::paint(juce::Graphics& g)
{
    DBG("keyboard position: " + std::to_string(keyboardComponent.getX()) + ", " + std::to_string(keyboardComponent.getY()));

    juce::ColourGradient gradient (juce::Colours::blueviolet, 200.0f, 0.0f,  juce::Colours::black, 0.0f, 600.0f, true);
    g.setGradientFill(gradient);
    g.setOpacity(0.8f);
    g.fillAll();
}

void UIComponents::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);

    tabbedComponent.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.75f);

    sampleSelector.setBoundsRelative(0.45, 0.5, 0.15, 0.06);
}