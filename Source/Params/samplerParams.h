#pragma once

#include <JuceHeader.h>

namespace nSamplerSound {
class SamplerParams
{
public:
    SamplerParams() {}
    ~SamplerParams() {}
    void setGain(float gainToUse) { gain = gainToUse; }
    float getGain() { return std::pow(10, gain / 20); }

    void setADSR(float attackToUse, float decayToUse, float sustainToUse, float releaseToUse);
    void setReverb(float roomToUse, float dampToUse, float widthToUse, float wetToUse, float dryToUse);

    juce::ADSR::Parameters getADSRParams() const { return adsrParams; }
    juce::Reverb::Parameters getReverbParams() { return reverbParams; }
private:
    float gain = { 0.0 };

    juce::ADSR::Parameters adsrParams;
    juce::Reverb::Parameters reverbParams;
};

}