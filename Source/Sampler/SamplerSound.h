#include <JuceHeader.h>
#include "../Filters/Filters.h"
#include "juce_dsp/juce_dsp.h"

namespace nSamplerSound {
    class SamplerSound : public SynthesiserSound {
    public:
        SamplerSound(const juce::String &name,
                     juce::AudioFormatReader &source,
                     const juce::BigInteger &midiNotes,
                     int midiNoteForNormalPitch,
                     double attackTimeSecs,
                     double releaseTimeSecs,
                     double maxSampleLengthSeconds);


        /** Destructor. */
        ~SamplerSound() override;

        //==============================================================================
        [[nodiscard]]
        const juce::String &getName() const

        noexcept { return name; }

        [[nodiscard]]
        juce::AudioBuffer<float> *getAudioData() const

        noexcept { return data.get(); }

        //==============================================================================

        void setReverbParameters(const juce::Reverb::Parameters &params) noexcept { reverbParams = params; }

        void setADSRParameters(const juce::ADSR::Parameters &params) noexcept { adsrParams = params; }


        void setGain(float newGain) noexcept { gain = newGain; }

        void setLowpassCutOff(float newLowpassCutOff) noexcept { lowpassCutOff = newLowpassCutOff; }

        void setHighpassCutOff(float newHighpassCutOff) noexcept { highpassCutOff = newHighpassCutOff; }

        void setFilterIndex(int newIndex) noexcept { filterIndex = newIndex; }

        bool appliesToNote(int midiNoteNumber) override;

        bool appliesToChannel(int midiChannel) override;

    private:
        //==============================================================================
        friend class SamplerVoice;

        String name;
        std::unique_ptr<juce::AudioBuffer<float>> data;
        double sourceSampleRate;
        BigInteger midiNotes;
        int length = 0, midiRootNote = 0;
        float gain = 0.0;
        int filterIndex = 0;
        float lowpassCutOff = 6000.0f;
        float highpassCutOff = 10000.0f;
        juce::dsp::Reverb::Parameters reverbParams;
        juce::ADSR::Parameters adsrParams;

        JUCE_LEAK_DETECTOR (SamplerSound)
    };

    class SamplerVoice : public juce::SynthesiserVoice {
    public:
        //==============================================================================
        /** Creates a SamplerVoice. */
        SamplerVoice();


        /** Destructor. */
        ~SamplerVoice() override;

        //==============================================================================
        bool canPlaySound(juce::SynthesiserSound *) override;

        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *, int pitchWheel) override;

        void stopNote(float velocity, bool allowTailOff) override;

        void pitchWheelMoved(int newValue) override;

        void controllerMoved(int controllerNumber, int newValue) override;

        void applyGainToBuffer (juce::AudioBuffer<float>& buffer, float gain);

        void renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples) override;

    private:
        //==============================================================================
        friend class SamplerSound;
        double pitchRatio = 0;
        double sourceSamplePosition = 0;
        float lgain = 0.0, rgain = 0.0;

        juce::dsp::ProcessSpec spec = {
            .sampleRate = 44100.0f,
            .maximumBlockSize = 512,
            .numChannels = 2
        };

        juce::ADSR adsr;
        juce::dsp::Reverb reverb;
        LowpassFilter lowpass;
        HighpassFilter highpass;
        std::atomic<int> filterIndex { 0 };
        juce::AudioBuffer<float> voiceBuffer; //{ 2, (44100 * 4) + 4 };

        JUCE_LEAK_DETECTOR (SamplerVoice)
    };
}
