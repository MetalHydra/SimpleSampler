#include "Sampler.h"

Sampler::Sampler(std::string name,  int numVoices) : name(name), numVoices(numVoices)
{
    audioFormatManager.registerBasicFormats();
    initializeSamplers();
}

void Sampler::initializeSamplers()
{
    for (int i = 0; i < pathToSampleFolders.size(); i++)
    {
        samplers.add(new juce::Synthesiser());
        juce::Array<int> arr;

        for (int j = 0; j < getNumVoices(); j++)
        {
            samplers[i]->addVoice(new nSamplerSound::SamplerVoice());
        }

        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (pathToSampleFolders[i], false))
        {
            //DBG("entry: " + entry.getFile().getFileName());
            auto file = entry.getFile();
            auto reader = audioFormatManager.createReaderFor(file);
            auto midiNumber = file.getFileNameWithoutExtension().getIntValue();
            arr.add(midiNumber);
            //DBG("midi number: " + std::to_string(midiNumber));
            samplers[i]->addSound(new nSamplerSound::SamplerSound(file.getFileNameWithoutExtension(), *reader, BigInteger().setBit(midiNumber,true), midiNumber, 0.2, 0.2, 4.0));
            delete reader;
        }
        //DBG("arr size: " + std::to_string(arr.size()));
        sampleMidiNumbers.add(arr);
    }
    //DBG("number midi arrays: " + std::to_string(sampleMidiNumbers.size()));
}

