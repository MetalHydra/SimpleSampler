#include "SynthVoice.h"

SynthVoice::SynthVoice()
{
}

SynthVoice::~SynthVoice()
{
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
   return dynamic_cast<juce::SamplerSound*>(sound) != nullptr;
}

void SynthVoice::setPitchBend(int pitchWheelPosition)
{
  if (pitchWheelPosition > 8192)
  {
    currentPitchBend = static_cast<float>(pitchWheelPosition - 8192) / (16834 - 8192);
  }
  else
  {
    currentPitchBend = static_cast<float>(pitchWheelPosition) / 8192;
  }
}

float SynthVoice::pitchBendCents(int semitones)
{
  if (currentPitchBend >= 0.0f)
  {
    return currentPitchBend * semitones * 100;
  }
  else
  {
    return currentPitchBend * semitones * 100;
  }
}

double const note2Freq(int midiNoteNumber, double centsOffset)
{
  double hertz = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
  hertz *= std::pow(2.0, centsOffset / 1200);
  return hertz;
}

void SynthVoice::updateADSRParams(juce::ADSR::Parameters &params)
{
	adsr.setParameters(params);
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    if (auto* sound = dynamic_cast<const SamplerSound*> (s))
    {
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                        * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
        lgain = velocity;
        rgain = velocity;

        adsr.setSampleRate (sound->sourceSampleRate);
        adsr.setParameters (sound->params);

        adsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
    }
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

    if (auto* playingSound = static_cast<SamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        auto& data = *playingSound->data;
        const float* const inL = data.getReadPointer (0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;

        while (--numSamples >= 0)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                                       : l;

            auto envelopeValue = adsr.getNextSample();

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }

            sourceSamplePosition += pitchRatio;

            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
        }
    }
/*
    for (int sample = 0; sample < numSamples; ++sample)
    {
       for(int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
       {
           outputBuffer.addSample(channel, startSample, outputBuffer.getSample(channel, startSample));
       }
       ++startSample;
    }*/
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
    {
        setPitchBend(newPitchWheelValue);
    }

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}