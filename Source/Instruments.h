#pragma once
#include <JuceHeader.h>
#include "Sampler/Sampler.h"

enum StringInstrumentPlayStyle
{
    normal = 0,
    palmMuted = 1
};

class Instruments
{
public:
    Instruments( std::string name,  int numVoices);

    ~Instruments() = default;

    [[nodiscard]]
    std::string getName() const { return name; }

    [[nodiscard]]
    int getNumVoices() const { return numVoices; }

    [[nodiscard]]
    int getNumSamplers() const { return samplers.size(); }

    void initializeSamplers();

    juce::Synthesiser& getSampler(int index) { return *samplers[index]; }

    juce::OwnedArray<juce::Synthesiser>& getSamplers() { return samplers; }

    void updateADSRParams(juce::ADSR::Parameters &params);

private:

    juce::Array<juce::File> pathToSampleFolders = { juce::File("~/Sampler/SimpleSampler/Source/Sounds/AcousticSamples/"),
                                                    juce::File("~/Sampler/SimpleSampler/Source/Sounds/AcousticSamplesPalmMuted/")};

    std::string name;
    int numVoices;

    juce::OwnedArray<juce::Synthesiser> samplers;
    juce::AudioFormatManager audioFormatManager;
    juce::ADSR adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Instruments)
};




