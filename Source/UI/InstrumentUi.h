#pragma once

#include<JuceHeader.h>

class InstrumentUi : public juce::Component
{
public:
    InstrumentUi();
    ~InstrumentUi() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    int numStrings = { 6 };
    int numFrets = { 20 };
    juce::Rectangle<int> emptyFretboard {5, 5, 1190, 200};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentUi)
};