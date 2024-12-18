/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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

void SimpleSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    updateSamplerIndex();
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    for (auto& sampler : currentSamplers)
    {
        sampler->setCurrentPlaybackSampleRate(sampleRate);
    }

    if (shouldUpdate)
    {
        updateParams();
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
        updateParams();
        shouldUpdate = false;
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    currentSamplers[currentSamplerIndex]->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}


bool SimpleSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleSamplerAudioProcessor::createEditor()
{
    return new SimpleSamplerAudioProcessorEditor (*this);
}

void SimpleSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SimpleSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

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
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>(0.1f, 2.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>(0.0f, 3.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>(-12.0f, 12.0f), -1.0f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>("SAMPLE", "Sample", sampleChoices, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ROOM", "Room", juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DAMP", "Damp", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET", "Wet", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("CutoffBand1", "CutoffBand1", juce::NormalisableRange<float>(1000.0f, 20000.0f), 3400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CutoffBand2", "CutoffBand2", juce::NormalisableRange<float>(1000.0f, 20000.0f), 3400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CutoffBand3", "CutoffBand3", juce::NormalisableRange<float>(1000.0f, 20000.0f), 3400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CutoffBand4", "CutoffBand4", juce::NormalisableRange<float>(1000.0f, 20000.0f), 3400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CutoffBand5", "CutoffBand5", juce::NormalisableRange<float>(1000.0f, 20000.0f), 3400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("QBand1", "QBand1", juce::NormalisableRange<float>(1.0f, 10.0f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("QBand2", "QBand2", juce::NormalisableRange<float>(1.0f, 10.0f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("QBand3", "QBand3", juce::NormalisableRange<float>(1.0f, 10.0f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("QBand4", "QBand4", juce::NormalisableRange<float>(1.0f, 10.0f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("QBand5", "QBand5", juce::NormalisableRange<float>(1.0f, 10.0f), 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FilterBand1", "FilterBand1", filterChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FilterBand2", "FilterBand2", filterChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FilterBand3", "FilterBand3", filterChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FilterBand4", "FilterBand4", filterChoices, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FilterBand5", "FilterBand5", filterChoices, 0));

    return { params.begin(), params.end() };
}

void SimpleSamplerAudioProcessor::updateParams()
{
    auto gain = APVTS.getRawParameterValue("GAIN")->load();
    auto attack = APVTS.getRawParameterValue("ATTACK")->load();
    auto decay = APVTS.getRawParameterValue("DECAY")->load();
    auto sustain = APVTS.getRawParameterValue("SUSTAIN")->load();
    auto release = APVTS.getRawParameterValue("RELEASE")->load();

    auto roomSize = APVTS.getRawParameterValue("ROOM")->load();
    auto damping = APVTS.getRawParameterValue("DAMP")->load();
    auto wetLevel = APVTS.getRawParameterValue("WET")->load();
    auto width = APVTS.getRawParameterValue("WIDTH")->load();

    //auto cutoff = APVTS.getRawParameterValue("CUTOFF")->load();
    //auto Q = APVTS.getRawParameterValue("Q")->load();
    //auto samplerate = static_cast<float>(getSampleRate());

    //auto filterIndex = APVTS.getRawParameterValue("FILTER")->load();
    //DBG("filter index: " + std::to_string(filterIndex));
    for (int i = 0; i < currentSamplers[currentSamplerIndex]->getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<nSamplerSound::SamplerSound*>(currentSamplers[currentSamplerIndex]->getSound(i).get()))
        {
            sound->setSpec(spec);
            sound->setGainParameters(gain, gain, false);
            sound->setReverbParameters(roomSize, damping, width, wetLevel, (1 - wetLevel));
            sound->setAdsrParameters(attack, decay, sustain, release);
        }
    }
}

// update sampler index and get current notes for recoloring the kayboard
void SimpleSamplerAudioProcessor::updateSamplerIndex()
{
    auto value = APVTS.getRawParameterValue("SAMPLE")->load();
    currentSamplerIndex = static_cast<int>(value);
    DBG("update sampler");
    auto midiNotes = sampler.getSampleMidiNumbers(currentSamplerIndex);
    DBG("changed sampler " + std::to_string(midiNotes.size()));
}

void SimpleSamplerAudioProcessor::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property)
{
    shouldUpdate = true;
}



