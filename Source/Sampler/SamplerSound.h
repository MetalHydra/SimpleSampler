#include <JuceHeader.h>
#include "../Params/samplerParams.h"
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

        void setParameters(SamplerParams &samplerParamsToUse) { samplerParams = samplerParamsToUse; }

        bool appliesToNote(int midiNoteNumber) override;

        bool appliesToChannel(int midiChannel) override;

        SamplerParams getSamplerParams()  { return samplerParams; }

    private:
        //==============================================================================
        friend class SamplerVoice;

        String name;
        std::unique_ptr<juce::AudioBuffer<float>> data;
        double sourceSampleRate;
        BigInteger midiNotes;
        int length = 0, midiRootNote = 0;

        SamplerParams samplerParams;

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
        //using juce::SynthesiserVoice::renderNextBlock;

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
        std::atomic<bool> isReady { false };
        juce::AudioBuffer<float> voiceBuffer; //{ 2, (44100 * 4) + 4 };

        JUCE_LEAK_DETECTOR (SamplerVoice)
    };
}
