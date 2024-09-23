
#pragma once
/*

This Class manages the SamplerSounds for a Synthesizer, given by a name
  Created by dennis on 13/09/2022.

*/

#include <JuceHeader.h>

class Instruments
{
public:
    Instruments(std::string name, const bool isStringInstrument, const int numStrings, const int numFrets, const int requiredSamplers, const int numVoices) : name(std::move(name)), isStringInstrument(isStringInstrument), isDrum(!isStringInstrument), numStrings(numStrings), numFrets(numFrets), requiredSamplers(requiredSamplers), numVoices(numVoices) {};

    ~Instruments() = default;

    [[nodiscard]]
    std::string getName() const { return name; }

    [[nodiscard]]
    bool getIsStringInstrument() const { return isStringInstrument; }

    [[nodiscard]]
    bool getIsDrum() const { return isDrum; }

    [[nodiscard]]
    int getNumStrings() const { return numStrings; }

    [[nodiscard]]
    int getNumFrets() const { return numFrets; }

    [[nodiscard]]
    int getRequiredSamplers() const { return requiredSamplers; }

    [[nodiscard]]
    int getNumVoices() const { return numVoices; }

private:
    const std::string name;
    const bool isStringInstrument;
    const bool isDrum;
    const int numStrings;
    const int numFrets;
    const int requiredSamplers;
    const int numVoices;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Instruments)
};

class AcousticGuitar : public Instruments
{
public:
    AcousticGuitar();

    ~AcousticGuitar() = default;
    void initializeSamplers();
    juce::Synthesiser& getSampler(const int i) { return *guitarSamplers[i]; }
    juce::OwnedArray<juce::Synthesiser>& getSamplers() { return guitarSamplers; }
private:
    juce::BigInteger allNotes;
    juce::OwnedArray<juce::Synthesiser> guitarSamplers;
    juce::Array<juce::File> samplerFiles = {juce::File("/home/dennis/Sampler/SimpleSampler/Source/Sounds/AcousticSamples/"), juce::File("/home/dennis/Sampler/SimpleSampler/Source/Sounds/acousticSamplesPalmMuted/") };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AcousticGuitar)
};