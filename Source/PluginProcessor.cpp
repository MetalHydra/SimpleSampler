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
    APVTS.state.addListener(this);
}

SimpleSamplerAudioProcessor::~SimpleSamplerAudioProcessor()
{
}

//returns the keyboardstate which is required for the initialisation of the keyboard

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
    updateSamplerIndex();

    for (auto& sampler : currentSamplers)
    {
        sampler->setCurrentPlaybackSampleRate(sampleRate);
    }

    if (shouldUpdate)
    {
        updateADSRParams();
        shouldUpdate = false;
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

    if (shouldUpdate)
    {
        updateSamplerIndex();
        updateADSRParams();
        shouldUpdate = false;
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    currentSamplers[currentSamplerIndex]->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());



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

juce::MidiKeyboardState& SimpleSamplerAudioProcessor::getKeyboardState()
{
    return keyboardState;
}

void SimpleSamplerAudioProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //DBG("note on pressed: " + std::to_string(midiNoteNumber));
}

void SimpleSamplerAudioProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //DBG("note on released: " + std::to_string(midiNoteNumber));
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleSamplerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>(-24.0f, 24.0f), -1.0f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("SAMPLE", "Sample", sampleChoices, 0));

    //applying Reverb parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ROOM", "Room", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET", "Wet", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY", "Dry", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREEZE", "Freeze", juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f));
    return { params.begin(), params.end() };
}

void SimpleSamplerAudioProcessor::updateADSRParams()
{
    adsrParams.attack = APVTS.getRawParameterValue("ATTACK")->load();
    adsrParams.decay = APVTS.getRawParameterValue("DECAY")->load();
    adsrParams.sustain = APVTS.getRawParameterValue("SUSTAIN")->load();
    adsrParams.release = APVTS.getRawParameterValue("RELEASE")->load();
    samplerParams.setGain(APVTS.getRawParameterValue("GAIN")->load());

    //applying Reverb parameters
    reverbParams.roomSize = APVTS.getRawParameterValue("ROOM")->load();
    reverbParams.damping = APVTS.getRawParameterValue("DAMP")->load();
    reverbParams.wetLevel = APVTS.getRawParameterValue("WET")->load();
    reverbParams.dryLevel = APVTS.getRawParameterValue("DRY")->load();
    reverbParams.width = APVTS.getRawParameterValue("WIDTH")->load();
    reverbParams.freezeMode = APVTS.getRawParameterValue("FREEZE")->load();


    for (int i = 0; i < currentSamplers[currentSamplerIndex]->getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<nSamplerSound::SamplerSound*>(currentSamplers[currentSamplerIndex]->getSound(i).get()))
        {
            sound->setEnvelopeParameters(adsrParams);
            sound->setParameters(samplerParams);
            sound->setReverbParameters(reverbParams);
        }
    }

    DBG("update params");
}

void SimpleSamplerAudioProcessor::updateSamplerIndex()
{
    auto value = APVTS.getRawParameterValue("SAMPLE")->load();

    currentSamplerIndex = static_cast<int>(value);
    DBG("Updated Sampler index " + std::to_string(currentSamplerIndex));
}


void SimpleSamplerAudioProcessor::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{
    shouldUpdate = true;
}



