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
                                                             eqComponent(p), adsrComponents(p), reverbComponent(p), instruments(p), tabbedComponent(juce::TabbedButtonBar::TabsAtTop) {
    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(tabbedComponent);
    tabbedComponent.addTab("Filters", juce::Colours::transparentBlack, &eqComponent, false);
    tabbedComponent.addTab("ADSR", juce::Colours::transparentBlack, &adsrComponents, false);
    tabbedComponent.addTab("Reverb", juce::Colours::transparentBlack, &reverbComponent, false);
    tabbedComponent.addTab("Instruments", juce::Colours::transparentBlack, &instruments, false);
}
UIComponents::~UIComponents()
{
}

void UIComponents::paint(juce::Graphics& g)
{
    DBG("keyboard position: " + std::to_string(keyboardComponent.getX()) + ", " + std::to_string(keyboardComponent.getY()));

    auto rectangle = keyboardComponent.getRectangleForKey(36);
    //DBG("rectangle: " + std::to_string(rectangle.getX()) + ", " + std::to_string(rectangle.getY()));
    // add keyboard y position to rectangle
    //rectangle.setY( keyboardComponent.getY());
    //rectangle.setX(rectangle.getX() + keyboardComponent.getX());
    //keyboardComponent.drawWhiteNote(40, g, rectangle, false, true, juce::Colours::black, juce::Colours::yellow);
    //g.setColour(juce::Colours::yellow);
    //juce::Rectangle<int> test(keyboardComponent.getX()+rectangle.getX(), keyboardComponent.getY(), rectangle.getWidth(), rectangle.getHeight());
    juce::ColourGradient gradient (juce::Colours::blueviolet, getWidth()/2.0f, 0.0f,  juce::Colours::darkslategrey, 0.0f, 0.0f, true);
    g.setGradientFill(gradient);
    g.setOpacity(0.8f);
    g.fillAll();
}

void UIComponents::resized()
{
    tabbedComponent.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.75f);
    keyboardComponent.setBoundsRelative(0.1f, 0.75f, 0.9f, 0.25);
}