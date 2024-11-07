#include "SamplerSound.h"


nSamplerSound::SamplerSound::SamplerSound (const juce::String& soundName,
                            juce::AudioFormatReader& source,
                            const juce::BigInteger& notes,
                            int midiNoteForNormalPitch,
                            double attackTimeSecs,
                            double releaseTimeSecs,
                            double maxSampleLengthSeconds)
    : name (soundName),
      sourceSampleRate (source.sampleRate),
      midiNotes (notes),
      midiRootNote (midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = jmin ((int) source.lengthInSamples,
                       (int) (maxSampleLengthSeconds * sourceSampleRate));

        data.reset (new AudioBuffer<float> (jmin (2, (int) source.numChannels), length + 4));
        doubleData.reset (new AudioBuffer<double> (jmin (2, (int) source.numChannels), length + 4));
        source.read (data.get(), 0, length + 4, 0, true, true);
        for (int channel = 0; channel < data->getNumChannels(); ++channel)
        {
            for (int i = 0; i < data->getNumSamples(); ++i)
            {
                doubleData->setSample (channel, i, data->getSample (channel, i));
            }
        }
        adsrParams.attack = static_cast<float>(attackTimeSecs);
        adsrParams.release = static_cast<float>(releaseTimeSecs);
    }
}

nSamplerSound::SamplerSound::~SamplerSound()
{

}


void nSamplerSound::SamplerSound::setGainParameters(float lGain, float rGain, bool useLinearGain)
{
    if (! useLinearGain)
    {
        gainParams.LGain = std::pow(10, lGain *0.05);
        gainParams.RGain = std::pow(10, rGain * 0.05);
    }
    else
    {
        gainParams.LGain = lGain;
        gainParams.RGain = rGain;
    }
}

void nSamplerSound::SamplerSound::setAdsrParameters(float attack, float decay, float sustain, float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
}

void nSamplerSound::SamplerSound::setFilterParameters(double cutOff, double Q, double samplerate, FilterType filterType)
{
    biquad.updateParamters(cutOff, Q, samplerate, filterType);
}

void nSamplerSound::SamplerSound::setReverbParameters(float room, float damp, float width, float wet, float dry)
{
    reverbParams.roomSize = room;
    reverbParams.damping = damp;
    reverbParams.width = width;
    reverbParams.wetLevel = wet;
    reverbParams.dryLevel = dry;
}

bool nSamplerSound::SamplerSound::appliesToNote (int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

bool nSamplerSound::SamplerSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

//==============================================================================
nSamplerSound::SamplerVoice::SamplerVoice() {}
nSamplerSound::SamplerVoice::~SamplerVoice() {}

bool nSamplerSound::SamplerVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<const SamplerSound*> (sound) != nullptr;
}

void nSamplerSound::SamplerVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{

    if (auto* sound = dynamic_cast<const SamplerSound*> (s))
    {
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                        * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
        reverb.prepare (sound->spec);
        reverb.setParameters (sound->reverbParams);
        adsr.setSampleRate (sound->sourceSampleRate);
        adsr.setParameters (sound->adsrParams);
        adsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void nSamplerSound::SamplerVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
        reverb.reset();
    }
}

void nSamplerSound::SamplerVoice::pitchWheelMoved (int /*newValue*/) {}

void nSamplerSound::SamplerVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/) {}

void nSamplerSound::SamplerVoice::applyGainToBuffer (juce::AudioBuffer<float>& buffer, float gain)
{
    buffer.applyGain (0, buffer.getNumSamples(), gain);
}

//==============================================================================
void nSamplerSound::SamplerVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
    if (auto* playingSound = static_cast<SamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        //adsr.setParameters(playingSound->samplerParams.getADSRParams());
        auto& data = *playingSound->data;

        const float* const inL = data.getReadPointer (0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;


        int cnt = 0;
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

            auto envelopeValue = adsr.getNextSample();
            l *=  envelopeValue;
            r *=  envelopeValue;
            if (outR != nullptr)
            {
                voiceBuffer.setSample(0, sample, l);
                voiceBuffer.setSample(1, sample, r);
                //*outL++ += l;
                //*outR++ += r;
            }
            else
            {
                voiceBuffer.setSample(0, sample, l + r);
                //*outL++ += (l + r) * 2.5f;
            }
            sourceSamplePosition += pitchRatio;

            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
            cnt++;
        }

        playingSound->biquad.ProcessBlock(voiceBuffer);

        reverb.setParameters(playingSound->reverbParams);
        auto audioBlock = juce::dsp::AudioBlock<float>(voiceBuffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

        reverb.process(context);

        voiceBuffer.applyGain(0, cnt, playingSound->gainParams.LGain);
        if (outR != nullptr)
        {
            for(int sample = 0; sample < cnt; ++sample)
            {
                *outL++ += voiceBuffer.getSample(0, sample);
                *outR++ += voiceBuffer.getSample(1, sample);
            }
        }
        else
        {
            for(int sample = 0; sample < cnt; ++sample)
            {
                *outL++ += voiceBuffer.getSample(0, sample);
            }
        }
    }
}

void nSamplerSound::SamplerVoice::renderNextBlock (juce::AudioBuffer<double>& outputBuffer, int startSample, int numSamples)
{
    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples);
    if (auto* playingSound = static_cast<SamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        //adsr.setParameters(playingSound->samplerParams.getADSRParams());
        auto& data = *playingSound->doubleData;

        const double * const inL = data.getReadPointer (0);
        const double* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        double* outL = outputBuffer.getWritePointer (0, startSample);
        double* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;


        int cnt = 0;
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            double l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            double r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

            auto envelopeValue = adsr.getNextSample();
            l *=  envelopeValue;
            r *=  envelopeValue;
            if (outR != nullptr)
            {
                voiceBuffer.setSample(0, sample, l);
                voiceBuffer.setSample(1, sample, r);
                //*outL++ += l;
                //*outR++ += r;
            }
            else
            {
                voiceBuffer.setSample(0, sample, l + r);
                //*outL++ += (l + r) * 2.5f;
            }
            sourceSamplePosition += pitchRatio;

            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
            cnt++;
        }

        playingSound->biquad.ProcessBlock(voiceBuffer);

        reverb.setParameters(playingSound->reverbParams);
        //auto audioBlock = juce::dsp::AudioBlock<double>(voiceBuffer);
        //auto context = juce::dsp::ProcessContextReplacing<double>(audioBlock);

        //reverb.process(context);

        voiceBuffer.applyGain(0, cnt, playingSound->gainParams.LGain);
        if (outR != nullptr)
        {
            for(int sample = 0; sample < cnt; ++sample)
            {
                *outL++ += voiceBuffer.getSample(0, sample);
                *outR++ += voiceBuffer.getSample(1, sample);
            }
        }
        else
        {
            for(int sample = 0; sample < cnt; ++sample)
            {
                *outL++ += voiceBuffer.getSample(0, sample);
            }
        }
    }
}