/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleSamplerAudioProcessor::SimpleSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),APVTS(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    keyboardState.addListener(this);
}

SimpleSamplerAudioProcessor::~SimpleSamplerAudioProcessor()
{
}

//returns the keyboardstate which is required for the initialisation of the keyboard
juce::MidiKeyboardState& SimpleSamplerAudioProcessor::getKeyboardState()
{
    return keyboardState;
}

void SimpleSamplerAudioProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    DBG("note on pressed: " + std::to_string(midiNoteNumber));
}

void SimpleSamplerAudioProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    DBG("note on released: " + std::to_string(midiNoteNumber));
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleSamplerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PM", "pm", false));
    return { params.begin(), params.end() };
}



void SimpleSamplerAudioProcessor::processMidiBuffer(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

}

//==============================================================================
const juce::String SimpleSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleSamplerAudioProcessor::getNumPrograms()
{
    return 1;
}

int SimpleSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //instrumentManager.getNormalSynth().setCurrentPlaybackSampleRate(sampleRate);
    //instrumentManager.getPalmMutedSynth().setCurrentPlaybackSampleRate(sampleRate);
    for (auto& sampler : currentSamplers)
    {
        sampler->setCurrentPlaybackSampleRate(sampleRate);

    }
}

void SimpleSamplerAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    bool isPalmMuted = APVTS.getRawParameterValue("PM")->load();
    DBG("is palm muted: " + std::to_string(isPalmMuted));

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /*for(int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // do some adsr stuff
        }
    }*/
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    //synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    /*if (isPalmMuted)
    {
        instrumentManager.getPalmMutedSynth().renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }
    else
    {
        instrumentManager.getNormalSynth().renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }*/
    if (isPalmMuted)
    {
        currentSamplers[1]->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }
    else
    {
        currentSamplers[0]->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SimpleSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleSamplerAudioProcessor::createEditor()
{
    return new SimpleSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SimpleSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleSamplerAudioProcessor();
}
