/*
  ==============================================================================

    BaseComponents.cpp
    Created: 13 Sep 2024 3:10:40pm
    Author:  dennis

  ==============================================================================
*/

#include<JuceHeader.h>
#include "BaseComponents.h"

CustomComponent::CustomComponent(SimpleSamplerAudioProcessor& p) : audioProcessor(p) , keyboardComponent(p.getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent); 
}

CustomComponent::~CustomComponent()
{
}

void CustomComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void CustomComponent::resized()
{
    keyboardComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25);
}