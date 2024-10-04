#include "Instruments.h"

Instruments::Instruments(std::string name,  int numVoices,  juce::Array<juce::File> pathToSamples) : name(name),
                                                                                                         numVoices(numVoices),
                                                                                                         pathToSamples(pathToSamples),
                                                                                                         requiredSamplers(pathToSamples.size())

{
    audioFormatManager.registerBasicFormats();
    initializeSamplers();
}

void Instruments::initializeSamplers()
{
    for (int i = 0; i < getRequiredSamplers(); i++)
    {
        samplers.add(new juce::Synthesiser());
        for (int j = 0; j < getNumVoices(); j++)
        {
            samplers[i]->addVoice(new juce::SamplerVoice());
        }

        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (pathToSamples[i], false))
        {
            DBG("entry: " + entry.getFile().getFileName());
            auto file = entry.getFile();
            //readers.add(audioFormatManager.createReaderFor(file));
            auto reader = audioFormatManager.createReaderFor(file);
            auto midiNumber = file.getFileNameWithoutExtension().getIntValue();
            DBG("midi number: " + std::to_string(midiNumber));
            samplers[i]->addSound(new juce::SamplerSound(file.getFileNameWithoutExtension(), *reader, BigInteger().setBit(midiNumber,true), midiNumber, 0.2, 0.2, 4.0));
            //samplers[i]->addSound(new MySamplerSound(file.getFileNameWithoutExtension(), *reader, BigInteger().setBit(midiNumber,true), midiNumber, 0.2, 0.2, 4.0));

            delete reader;
        }
    }
    DBG("number of Synths: " + std::to_string(samplers.size()));
}


StringInstrument::StringInstrument(std::string name, int numVoices, juce::Array<juce::File> pathToSamples, int numStrings, int numFrets) :
                                                                                                                Instruments(name,  numVoices, pathToSamples),
                                                                                                                            numStrings(numStrings), numFrets(numFrets)
{

}
