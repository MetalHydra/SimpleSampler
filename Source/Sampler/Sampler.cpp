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
        for (int j = 0; j < getNumVoices(); j++)
        {
            samplers[i]->addVoice(new nSamplerSound::SamplerVoice());
        }

        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (pathToSampleFolders[i], false))
        {
            DBG("entry: " + entry.getFile().getFileName());
            auto file = entry.getFile();
            //readers.add(audioFormatManager.createReaderFor(file));
            auto reader = audioFormatManager.createReaderFor(file);
            auto midiNumber = file.getFileNameWithoutExtension().getIntValue();
            DBG("midi number: " + std::to_string(midiNumber));
            //samplers[i]->addSound(new juce::SamplerSound(file.getFileNameWithoutExtension(), *reader, BigInteger().setBit(midiNumber,true), midiNumber, 0.2, 0.2, 4.0));
            samplers[i]->addSound(new nSamplerSound::SamplerSound(file.getFileNameWithoutExtension(), *reader, BigInteger().setBit(midiNumber,true), midiNumber, 0.2, 0.2, 4.0));

            delete reader;
        }
    }
    DBG("number of Synths: " + std::to_string(samplers.size()));
}

