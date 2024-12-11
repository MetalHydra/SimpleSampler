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
    filterBand1.setBoundsRelative(0.01f, 0.55f, 0.19f, 0.4f);
    filterBand2.setBoundsRelative(0.01f + 0.17f + 0.025f, 0.55f, 0.19f, 0.4f);
    filterBand3.setBoundsRelative(0.01f + 2 * (0.17f + 0.025f), 0.55f, 0.19f, 0.4f);
    filterBand4.setBoundsRelative(0.01f + 3 * (0.17f + 0.025f), 0.55f, 0.19f, 0.4f);
    filterBand5.setBoundsRelative(0.01f + 4 * (0.17f + 0.025f), 0.55f, 0.19f, 0.4f);
}