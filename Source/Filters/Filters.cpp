#include "Filters.h"

BiQuadFilter::BiQuadFilter()
{

}

BiQuadFilter::~BiQuadFilter()
{

}

void BiQuadFilter::updateParamters(float cutoff, float Q, float samplerate, FilterType filter)
{
    parameters.fc = cutoff;
    parameters.Q = Q;
    parameters.samplerate = samplerate;
    calculateCoefficients(filter);
}

void BiQuadFilter::calculateCoefficients(FilterType filter) {

    switch (filter)
    {
        case LOWPASS:
        {
            DBG("CALCULATE LOWPASS");
            float theta = 2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate;
            float d = 1.0f / parameters.Q;
            float num = 1.0f - (d/2.0f) * std::sin(theta);
            float denum = 1.0f + (d/2.0f) * std::sin(theta);
            float beta = 0.5f * (num / denum);
            float gamma = (0.5f + beta) * std::cos(theta);
            coefficients.a0 = (0.5f + beta - gamma) / 2.0f;
            coefficients.a1 = 0.5f + beta - gamma;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -2.0f *gamma;
            coefficients.b2 = 2.0f * beta;
            break;
        }
        case HIGHPASS:
        {
            DBG("CALCULATE HIGHPASS");
            float theta = 2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate;
            float d = 1.0f / parameters.Q;
            float num = 1.0f - (d/2.0f) * std::sin(theta);
            float denum = 1.0f + (d/2.0f) * std::sin(theta);
            float beta = 0.5f * (num / denum);
            float gamma = (0.5f + beta) * std::cos(theta);
            coefficients.a0 = (0.5f + beta + gamma) / 2.0f;
            coefficients.a1 = 0.5f + beta + gamma;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -2.0f *gamma;
            coefficients.b2 = 2.0f * beta;
            break;
        }
        case BANDPASS:
        {
            DBG("CALCULATE BANDBPASS");
            float K = std::tan((juce::MathConstants<float>::pi * parameters.fc) / parameters.samplerate);
            float delta = (K*K*parameters.Q) + K + parameters.Q;
            coefficients.a0 = K / delta;
            coefficients.a1 = 0.0f;
            coefficients.a2 = -K/delta;
            coefficients.b1 = ((2.0f * parameters.Q)*(K*K-1)) / delta;
            coefficients.b2 = ((K*K*parameters.Q)-K+parameters.Q) / delta;
            break;
        }

    }
}

void BiQuadFilter::reset()
{
    x1State = {0.0f, 0.0f};
    x2State = {0.0f, 0.0f};
    y1State = {0.0f, 0.0f};
    y2State = {0.0f, 0.0f};
}

void BiQuadFilter::ProcessBlock(juce::AudioBuffer<float>& buffer)
{
    //LeftChannel 01, RightChannel 2,3
    /*x1State.resize(buffer.getNumChannels(), 0.0f);
    x2State.resize(buffer.getNumChannels(), 0.0f);
    y1State.resize(buffer.getNumChannels(), 0.0f);
    y2State.resize(buffer.getNumChannels(), 0.0f);*/
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto out = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = out[i];
            //DBG("size of x1, x2, y1, y2: " << x1State.size() << ", " << x2State.size() << ", " << y1State.size() << ", " << y2State.size());
            const auto filterdSample = coefficients.a0 * inputSample + x1State[channel];
            //const auto filtredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel] + coefficients.a2 * x2State[channel] - coefficients.b1 * y1State[channel] - coefficients.b2 * y2State[channel];
            //const auto filteredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel] - coefficients.b1 * y1State[channel]; //#auto filteredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel];
            x1State[channel] = coefficients.a1 * inputSample - coefficients.b1 * filterdSample + x2State[channel];
            x2State[channel] = coefficients.a2 * inputSample - coefficients.b2 * filterdSample;

            //y2State[channel] = y1State[channel];
            //y1State[channel] = filterdSample;
            out[i] = filterdSample;
        }
    }
}


