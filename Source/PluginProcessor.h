/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Sampler/Sampler.h"


class SimpleSamplerAudioProcessor  : public juce::AudioProcessor,
                                        public juce::MidiKeyboardState::Listener,
                                        public juce::ValueTree::Listener
{
public:
    SimpleSamplerAudioProcessor();
    ~SimpleSamplerAudioProcessor() override;

    juce::MidiKeyboardState& getKeyboardState();

    juce::AudioProcessorValueTreeState& getAPVTS() { return APVTS; }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void addParameterToTree(std::string parameterName);

    void updateParams();

    void updateSamplerIndex();

    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property ) override;

private:
    const StringArray sampleChoices = { "Sample1", "Sample2" };
    const StringArray filterChoices = { "Lowpass", "Highpass", "Bandpass" };
    juce::MidiKeyboardState keyboardState;
    juce::AudioProcessorValueTreeState APVTS;
    Sampler sampler = Sampler("Sampler", 4);
    juce::OwnedArray<juce::Synthesiser>& currentSamplers = sampler.getSamplers();
    std::atomic<bool> shouldUpdate = { true };
    std::atomic<int> currentSamplerIndex = { 0 };
    juce::dsp::ProcessSpec spec;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSamplerAudioProcessor)
};
