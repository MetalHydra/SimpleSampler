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
    Instruments( std::string name,  int numVoices,  juce::Array<juce::File> pathToSamples);

    ~Instruments() = default;

    [[nodiscard]]
    std::string getName() const { return name; }

    [[nodiscard]]
    int getRequiredSamplers() const { return requiredSamplers; }

    [[nodiscard]]
    int getNumVoices() const { return numVoices; }

    [[nodiscard]]
    int getNumSamplers() const { return samplers.size(); }

    void initializeSamplers();

    juce::Synthesiser& getSampler(int index) { return *samplers[index]; }

    juce::OwnedArray<juce::Synthesiser>& getSamplers() { return samplers; }

    void updateADSRParams(juce::ADSR::Parameters &params);

private:
    std::string name;
    int numVoices;
    juce::Array<juce::File> pathToSamples;
    int requiredSamplers;
    juce::OwnedArray<juce::Synthesiser> samplers;
    juce::AudioFormatManager audioFormatManager;
    juce::ADSR adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Instruments)
};

class StringInstrument : public Instruments
{
public:
    StringInstrument(std::string name, int numVoices, juce::Array<juce::File> pathToSamples, int numStrings, int numFrets);
    static bool isStringInstrument() { return true; }
    static bool isDrumInstrument() { return false; }

    [[nodiscard]]
    int getNumStrings() const { return numStrings; }

    [[nodiscard]]
    int getNumFrets() const { return numFrets; }

private:
    int numStrings;
    int numFrets;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringInstrument)
};


