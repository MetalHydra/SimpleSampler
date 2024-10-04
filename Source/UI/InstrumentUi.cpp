#include "InstrumentUi.h"


InstrumentUi::InstrumentUi()
{

}

InstrumentUi::~InstrumentUi()
{
    // Add your code here
}

void InstrumentUi::paint(Graphics& g)
{
    g.setColour (juce::Colours::brown);

    g.fillRect(emptyFretboard);

}
void InstrumentUi::resized()
{
    // Add your code here
}