#pragma once

#include <JuceHeader.h>

class SamplerParams
{
public:
    SamplerParams() {};
    ~SamplerParams() {};
    void setGain(float gainToUse) { gain = gainToUse; };
    float getGain() { return std::pow(10, gain / 20); };

private:
    float gain = { 0.0 };
};