#pragma once
#include <JuceHeader.h>

enum FilterType
{
    LOWPASS1stOrder,
    HIGHPASS1stOrder,
    LOWPASS2ndOrder,
    HIGHPASS2ndOrder,
    BANDPASS2ndOrder,
    BANDSTOP2ndOrder,
    LOWPASSBUTTERWORTH,
    HIGHPASSBUTTERWORTH,
    BANDPASSBUTTERWORTH,
    BANDSTOPBUTTERWORTH,
    AllPASSFFILTER,
    ALPASSFILTER2ndOrder
};

struct FilterParameters
{
    float fc = 1000.0f;
    float Q = 0.707f;
    float samplerate = 44100.0f;
    FilterType filterType = LOWPASS2ndOrder;
};

struct filterCoefficients
{
    float a0 = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
    float c0 = 1.0f;
    float d0 = 0.0f;
};

class BiQuadFilter
{
public:
    BiQuadFilter();
    ~BiQuadFilter();
    void updateParamters(float cutoff, float Q, float samplerate, FilterType filterType);
    filterCoefficients& getFilterParamters();
    void ProcessBlock(juce::AudioBuffer<float>& buffer);
    virtual void calculateCoefficients(FilterType filter);
    void reset();

private:
    filterCoefficients coefficients;
    FilterParameters parameters;
    std::vector<float> x1State = {0.0f, 0.0f};
    std::vector<float> x2State = {0.0f, 0.0f};
    std::vector<float> y1State = {0.0f, 0.0f};
    std::vector<float> y2State = {0.0f, 0.0f};

    JUCE_LEAK_DETECTOR(BiQuadFilter)
};
