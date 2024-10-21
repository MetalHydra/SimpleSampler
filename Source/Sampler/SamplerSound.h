#include <JuceHeader.h>
#include "../Filters/Filters.h"
#include "juce_dsp/juce_dsp.h"

namespace nSamplerSound {

    struct gainParameters
    {
        float LGain = 0.0f;
        float RGain = 0.0f;
    };

    struct filterParameters
    {
        int filterIndex;
        float lowpassCutOff;
        float highpassCutOff;
    };

    class SamplerSound : public SynthesiserSound
    {
        public:
            SamplerSound(const juce::String &name,
                         juce::AudioFormatReader &source,
                         const juce::BigInteger &midiNotes,
                         int midiNoteForNormalPitch,
                         double attackTimeSecs,
                         double releaseTimeSecs,
                         double maxSampleLengthSeconds);

            ~SamplerSound() override;

            [[nodiscard]] const juce::String &getName() const noexcept { return name; }

            [[nodiscard]] juce::AudioBuffer<float> *getAudioData() const noexcept { return data.get(); }

            void setGainParameters(float lGain, float rGain, bool useLinearGain);

            void setFilterParameters(int filterIndex, float lowpassCutOff, float highpassCutOff);

            void setAdsrParameters(float attack, float decay, float sustain, float release);

            void setReverbParameters(float room, float damp, float width, float wet, float dry);

            void setSpec(juce::dsp::ProcessSpec& spec) { this->spec = spec; }


        bool appliesToNote(int midiNoteNumber) override;

            bool appliesToChannel(int midiChannel) override;

        private:

            friend class SamplerVoice;

            String name;
            std::unique_ptr<juce::AudioBuffer<float>> data;
            double sourceSampleRate;
            BigInteger midiNotes;
            int length = 0, midiRootNote = 0;

            gainParameters gainParams;
            filterParameters filterParams;
            juce::String filterType;
            juce::dsp::Reverb::Parameters reverbParams;
            juce::ADSR::Parameters adsrParams;

            juce::dsp::ProcessSpec spec;


            JUCE_LEAK_DETECTOR (SamplerSound)
    };

    class SamplerVoice : public juce::SynthesiserVoice {
    public:
        SamplerVoice();

        ~SamplerVoice() override;

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



        juce::ADSR adsr;
        juce::dsp::Reverb reverb;
        LowpassFilter lowpass;
        HighpassFilter highpass;
        std::atomic<int> filterIndex { 0 };
        // structure of each sample: { 2, (44100 * 4) + 4 };
        juce::AudioBuffer<float> voiceBuffer;

        JUCE_LEAK_DETECTOR (SamplerVoice)
    };
}
