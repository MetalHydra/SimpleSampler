#include "Instruments.h"

AcousticGuitar::AcousticGuitar() : Instruments("AcousticGuitar", true, 6, 20, 2, 4)
{
    allNotes.setRange(0, 128, true);
    initializeSamplers();
}

void AcousticGuitar::initializeSamplers()
{
    const auto audioFormatManager = std::make_unique<juce::AudioFormatManager>();
    audioFormatManager->registerBasicFormats();

    for (int i = 0; i < getRequiredSamplers(); i++)
    {
        //auto synth = std::make_unique<juce::Synthesiser>();
        //guitarSamplers.add(synth.release());
        guitarSamplers.add(new juce::Synthesiser());
        for (int j = 0; j < getNumVoices(); j++)
        {
            //auto voice = std::make_unique<juce::SamplerVoice>();
            //synth.addVoice();
            //synth.addVoice(voice.release());
            //guitarSamplers[i]->addVoice(voice.release());
            guitarSamplers[i]->addVoice(new juce::SamplerVoice());
        }

        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (samplerFiles[i], false))
        {
            DBG("entry: " + entry.getFile().getFileName());
            auto file = entry.getFile();
            auto reader = audioFormatManager->createReaderFor(file);
            guitarSamplers[i]->addSound(new juce::SamplerSound(file.getFileNameWithoutExtension(), *reader, allNotes, file.getFileNameWithoutExtension().getIntValue(), 0.2, 0.2, 4.0));
        }
    }
    DBG("number of Synths: " + std::to_string(guitarSamplers.size()));
}
