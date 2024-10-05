#include <JuceHeader.h>
#include "../Params/samplerParams.h"

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
        void setEnvelopeParameters(juce::ADSR::Parameters parametersToUse) { params = parametersToUse; }

        void setParameters(const SamplerParams &samplerParamsToUse) { samplerParams = samplerParamsToUse; }

        //==============================================================================
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

        ADSR::Parameters params;
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

        void renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples) override;
        //using juce::SynthesiserVoice::renderNextBlock;

    private:
        //==============================================================================
        double pitchRatio = 0;
        double sourceSamplePosition = 0;
        float lgain = 0.0, rgain = 0.0;
        ADSR adsr;


        JUCE_LEAK_DETECTOR (SamplerVoice)
    };
}
