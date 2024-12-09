//
// Created by dennis on 08.12.24.
//

#include "EQ.h"

EQ::EQ(SimpleSamplerAudioProcessor& p) : audioProcessor(p)
{
    addAndMakeVisible(filterBand1);
    addAndMakeVisible(filterBand2);
    addAndMakeVisible(filterBand3);
    addAndMakeVisible(filterBand4);
    addAndMakeVisible(filterBand5);
}

EQ::~EQ() {}

void EQ::paint(juce::Graphics &g)
{

}

void EQ::resized()
{
    DBG("filter width" + String(filterBand1.getWidth()));
    filterBand1.setBoundsRelative(0.1f,  0.2f, 0.2f, 0.35f);
    filterBand2.setBoundsRelative(0.4f, 0.2f, 0.1f, 0.4f);
}