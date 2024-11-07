#include "Filters.h"

BiQuadFilter::BiQuadFilter()
{

}

BiQuadFilter::~BiQuadFilter()
{

}

void BiQuadFilter::updateParamters(double cutoff, double Q, double samplerate, FilterType filter)
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
            auto theta = 2.0f * juce::MathConstants<double>::pi * parameters.fc / parameters.samplerate;
            auto gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1 - gamma) / 2.0f;
            coefficients.a1 = coefficients.a0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            break;
        }
        case HIGHPASS:
        {
            DBG("CALCULATE HIGHPASS");
            auto theta = 2.0f * juce::MathConstants<double>::pi * parameters.fc / parameters.samplerate;
            auto gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1 + gamma) / 2.0f;
            coefficients.a1 = -coefficients.a0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            break;
        }
        case BANDPASS:
        {
            DBG("CALCULATE BANDBPASS");
            break;
        }

    }
}

void BiQuadFilter::ProcessBlock(juce::AudioBuffer<float>& buffer)
{
    //LeftChannel 01, RightChannel 2,3
    x1State.resize(buffer.getNumChannels(), 0.0f);
    x2State.resize(buffer.getNumChannels(), 0.0f);
    y1State.resize(buffer.getNumChannels(), 0.0f);
    y2State.resize(buffer.getNumChannels(), 0.0f);
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto out = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = out[i];
            DBG("size of x1, x2, y1, y2: " << x1State.size() << ", " << x2State.size() << ", " << y1State.size() << ", " << y2State.size());
            const auto filtredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel] + coefficients.a2 * x2State[channel] - coefficients.b1 * y1State[channel] - coefficients.b2 * y2State[channel];
            //const auto filteredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel] - coefficients.b1 * y1State[channel]; //#auto filteredSample = coefficients.a0 * inputSample + coefficients.a1 * x1State[channel];
            x2State[channel] = x1State[channel];
            x1State[channel] = out[i];
            y2State[channel] = y1State[channel];
            y1State[channel] = filtredSample;
            out[i] = filtredSample;
        }
    }
}


