/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class SimpleSamplerAudioProcessor  : public juce::AudioProcessor,
                                        public juce::MidiKeyboardState::Listener
{
public:
    //==============================================================================
    SimpleSamplerAudioProcessor();
    ~SimpleSamplerAudioProcessor() override;

    //==============================================================================

    juce::MidiKeyboardState& getKeyboardState();

    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void loadAudioFile(juce::File file);

    void changeSynthesizerSounds(juce::SynthesiserSound* newSound);
    void processMidiBuffer(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    juce::AudioProcessorValueTreeState& getAPVTS() { return APVTS; }

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::Synthesiser synth;
    juce::MidiKeyboardState keyboardState;
    int numVoices = { 6 };
    juce::AudioProcessorValueTreeState APVTS;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleSamplerAudioProcessor)
};
