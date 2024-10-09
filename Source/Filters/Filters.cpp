#include "Filters.h"

LowpassFilter::LowpassFilter() {
    cutOff = 0.0f;
    samplerate = 0.0f;
}

LowpassFilter::~LowpassFilter() {
    dnBuffer.clear();
}

void LowpassFilter::setSamplingRate(float samplerate) {
    this->samplerate = samplerate;
}

void LowpassFilter::setCutOffFrequency(float cutoff) {
    this->cutOff = cutoff;
}

void LowpassFilter::processBlock(juce::AudioBuffer<float>& buffer)
{
    const float sign = 1.0f;
    dnBuffer.resize(buffer.getNumChannels(), 0.0f);
    const auto tan = std::tan(juce::MathConstants<float>::pi * cutOff / samplerate);
    const auto a1 = (tan - 1.0f) / (tan + 1.0f);
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto out = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
          const auto inputSample = out[i];
          const auto filtredSample = a1 * inputSample + dnBuffer[channel];
          dnBuffer[channel] = inputSample - a1 * filtredSample;
          const auto output = 0.5f * (inputSample + sign * filtredSample);
          out[i] = output;
        }
    }
}

HighpassFilter::HighpassFilter() {
    cutOff = 0.0f;
    samplerate = 0.0f;
}

HighpassFilter::~HighpassFilter() {
    dnBuffer.clear();
}

void HighpassFilter::setSamplingRate(float samplerate) {
    this->samplerate = samplerate;
}

void HighpassFilter::setCutOffFrequency(float cutoff) {
    this->cutOff = cutoff;
}

void HighpassFilter::processBlock(juce::AudioBuffer<float>& buffer)
{
    const float sign = -1.0f;
    dnBuffer.resize(buffer.getNumChannels(), 0.0f);
    const auto tan = std::tan(juce::MathConstants<float>::pi * cutOff / samplerate);
    const auto a1 = (tan - 1.0f) / (tan + 1.0f);
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto out = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = out[i];
            const auto filtredSample = a1 * inputSample + dnBuffer[channel];
            dnBuffer[channel] = inputSample - a1 * filtredSample;
            const auto output = 0.5f * (inputSample + sign * filtredSample);
            out[i] = output;
        }
    }
}

