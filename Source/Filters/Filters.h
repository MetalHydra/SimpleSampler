#include <JuceHeader.h>

enum FilterType
{
    Lowpass,
    Highpass,
    Bandpass
};

struct FilterParameters
{
    float fc = 100.0f;
    float Q = 0.707f;
    float cutoff = 0.0f;
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

class BiQaudFilter
{
public:
    BiQaudFilter();
    ~BiQaudFilter();
    void setCutOffFrequency(float cutoff);
    void setSamplingRate(float samplerate);
    void updateParamters(FilterParameters& parameters);
    filterCoefficients& getFilterParamters();
    void processBlock(juce::AudioBuffer<float>& buffer);
    void calculateCoefficients(FilterType filter);

private:
    filterCoefficients coefficients;
};

class LowpassFilter
{
public:
    LowpassFilter();
    ~LowpassFilter();
    void setCutOffFrequency(float cutoff);
    void setSamplingRate(float samplerate);
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
  float cutOff = 0.0f;
  float samplerate = 44100.0f;
  std::vector<float> dnBuffer;

    JUCE_LEAK_DETECTOR(LowpassFilter)
};

class HighpassFilter
{
public:
    HighpassFilter();
    ~HighpassFilter();
    void setCutOffFrequency(float cutoff);
    void setSamplingRate(float samplerate);
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    float cutOff = 0.0f;
    float samplerate = 44100.0f;
    std::vector<float> dnBuffer;

    JUCE_LEAK_DETECTOR(HighpassFilter)
};