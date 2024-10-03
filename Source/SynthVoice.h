#pragma once

#include <JuceHeader.h>

class SynthVoice : public juce::SynthesiserVoice
{
public:
      SynthVoice();
      ~SynthVoice() override;
      bool canPlaySound(juce::SynthesiserSound* sound) override;
      void setPitchBend(int pitchWheelPosition);
      float pitchBendCents(int semitones);
      double const note2Freq(int midiNoteNumber, double centsOffset);
      void updateADSRParams(juce::ADSR::Parameters &params);
      void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
      void stopNote(float velocity, bool allowTailOff) override;
      void pitchWheelMoved(int) override;
      void controllerMoved(int, int) override;
      void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
      float currentPitchBend = 0.0f;
      juce::ADSR adsr;
      double sourceSampleRate;
      BigInteger midiNotes;
      double pitchRatio = 0;
      double sourceSamplePosition = 0;
      float lgain = 0, rgain = 0;
      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoice)
};

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;
    SamplerSound* getSamplerSound()
    {
        return static_cast<SamplerSound*> (getSound());
    }
};