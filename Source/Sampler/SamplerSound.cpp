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

        source.read (data.get(), 0, length + 4, 0, true, true);
        adsrParams.attack = static_cast<float>(attackTimeSecs);
        adsrParams.release = static_cast<float>(releaseTimeSecs);
    }
}

nSamplerSound::SamplerSound::~SamplerSound()
{
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
    reverb.prepare (spec);
    if (auto* sound = dynamic_cast<const SamplerSound*> (s))
    {
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                        * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
        lgain = velocity;
        rgain = velocity;

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

        lowpass.setSamplingRate(playingSound->sourceSampleRate);
        highpass.setSamplingRate(playingSound->sourceSampleRate);
        lowpass.setCutOffFrequency(playingSound->lowpassCutOff);
        highpass.setCutOffFrequency(playingSound->highpassCutOff);

        int cnt = 0;
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto pos = (int) sourceSamplePosition;
            auto alpha = (float) (sourceSamplePosition - pos);
            auto invAlpha = 1.0f - alpha;

            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

            auto envelopeValue = adsr.getNextSample();
            lgain = playingSound->gain;
            rgain = lgain;
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

        switch (playingSound->filterIndex)
        {
            case 1:
                lowpass.processBlock(voiceBuffer);
                 DBG("Lowpass");
                break;
            case 2:
                highpass.processBlock(voiceBuffer);
                DBG("Lowpass");
                break;
            case 3:
                highpass.processBlock(voiceBuffer);
                lowpass.processBlock(voiceBuffer);
                DBG("Bandpass");
                break;
            default:
                break;
        }

        reverb.setParameters(playingSound->reverbParams);
        auto audioBlock = juce::dsp::AudioBlock<float>(voiceBuffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

        reverb.process(context);

        voiceBuffer.applyGain(0, cnt, lgain);
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
