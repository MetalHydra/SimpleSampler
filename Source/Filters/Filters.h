#pragma once
#include <JuceHeader.h>

enum FilterType
{
    LOWPASS,
    HIGHPASS,
    BANDPASS
};

struct FilterParameters
{
    double fc = 1000.0f;
    double Q = 0.707f;
    double samplerate = 44100.0f;
    FilterType filterType = LOWPASS;
};

struct filterCoefficients
{
    double a0 = 0.0f;
    double a1 = 0.0f;
    double a2 = 0.0f;
    double b1 = 0.0f;
    double b2 = 0.0f;
    double c0 = 1.0f;
    double d0 = 0.0f;
};

class BiQuadFilter
{
public:
    BiQuadFilter();
    ~BiQuadFilter();
    void updateParamters(double cutoff, double Q, double samplerate, FilterType filterType);
    filterCoefficients& getFilterParamters();
    void ProcessBlock(juce::AudioBuffer<float>& buffer);
    void calculateCoefficients(FilterType filter);

private:
    filterCoefficients coefficients;
    FilterParameters parameters;
    std::vector<double> x1State;
    std::vector<double> x2State;
    std::vector<double> y1State;
    std::vector<double> y2State;

    JUCE_LEAK_DETECTOR(BiQuadFilter)
};
