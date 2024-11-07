#pragma once
#include <JuceHeader.h>
#include "SamplerSound.h"

class Sampler
{
public:
    Sampler( std::string name,  int numVoices);

    ~Sampler() = default;

    [[nodiscard]] std::string getName() const { return name; }

    [[nodiscard]] int getNumVoices() const { return numVoices; }

    [[nodiscard]] int getNumSamplers() const { return samplers.size(); }

    void initializeSamplers();

    juce::Synthesiser& getSampler(int index) { return *samplers[index]; }

    juce::OwnedArray<juce::Synthesiser>& getSamplers() { return samplers; }



private:
    juce::Array<juce::File> pathToSampleFolders = { juce::File("~/Sampler/SimpleSampler/Source/Sounds/AcousticSamples/"),
                                                    juce::File("~/Sampler/SimpleSampler/Source/Sounds/AcousticSamplesPalmMuted/")};

    std::string name;
    int numVoices;

    juce::OwnedArray<juce::Synthesiser> samplers;
    juce::AudioFormatManager audioFormatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sampler)
};




