#include <JuceHeader.h>

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