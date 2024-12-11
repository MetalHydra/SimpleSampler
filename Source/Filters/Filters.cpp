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
        case LOWPASS1stOrder:
        {
            float theta = 2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate;
            float gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1.0f - gamma) / 2.0f;
            coefficients.a1 = coefficients.a0;
            coefficients.a2 = 0.0f;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            coefficients.c0 = 1.0f;
            coefficients.d0 = 0.0f;
            coefficients.order = 1;
            break;
        }
        case HIGHPASS1stOrder: {
            float theta = 2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate;
            float gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1.0f + gamma) / 2.0f;
            coefficients.a1 = -coefficients.a0;
            coefficients.a2 = 0.0f;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            coefficients.c0 = 1.0f;
            coefficients.d0 = 0.0f;
            coefficients.order = 1;
            break;
        }
        case LOWPASS2ndOrder:
        {
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
            coefficients.order = 2;
            break;
        }
        case HIGHPASS2ndOrder:
        {
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
            coefficients.order = 2;
            break;
        }
        case BANDPASS2ndOrder:
        {
            float K = std::tan((juce::MathConstants<float>::pi * parameters.fc) / parameters.samplerate);
            float delta = (K*K*parameters.Q) + K + parameters.Q;
            coefficients.a0 = K / delta;
            coefficients.a1 = 0.0f;
            coefficients.a2 = -K/delta;
            coefficients.b1 = ((2.0f * parameters.Q)*(K*K-1)) / delta;
            coefficients.b2 = ((K*K*parameters.Q)-K+parameters.Q) / delta;
            coefficients.order = 2;
            break;
        }
        case BANDSTOP2ndOrder: {
            float K = std::tan((juce::MathConstants<float>::pi * parameters.fc) / parameters.samplerate);
            float delta = (K*K*parameters.Q) + K + parameters.Q;
            coefficients.a0 = parameters.Q*(K*K+1.0f) / delta;
            coefficients.a1 = parameters.Q*(K*K-1.0f) / delta;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = coefficients.a1;
            coefficients.b2 = ((K*K*parameters.Q)-K+parameters.Q) / delta;
            coefficients.order = 2;
        }
        case LOWPASSBUTTERWORTH:
        {
            float C = 1.0f /  std::tan((juce::MathConstants<float>::pi * parameters.fc) / parameters.samplerate);
            coefficients.a0 = 1.0f / (1.0f + std::sqrt(2*C) + C*C);
            coefficients.a1 = 2.0f * coefficients.a0;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = 2.0f * coefficients.a0 * (1.0f - C*C);
            coefficients.b2 = coefficients.a0 * (1.0f - std::sqrt(2*C) + C*C);
            coefficients.order = 2;
            break;
        }
        case HIGHPASSBUTTERWORTH:
        {
            float C = 1.0f /  std::tan((juce::MathConstants<float>::pi * parameters.fc) / parameters.samplerate);
            coefficients.a0 = 1.0f / (1.0f + std::sqrt(2*C) + C*C);
            coefficients.a1 = -2.0f * coefficients.a0;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = 2.0f * coefficients.a0 * (C*C - 1.0f);
            coefficients.b2 = coefficients.a0 * (1.0f - std::sqrt(2*C) + C*C);
            coefficients.order = 2;
            break;
        }
        case BANDPASSBUTTERWORTH:
        {
            float BW = parameters.fc / parameters.Q;
            float C = 1 / std::tan((juce::MathConstants<float>::pi * parameters.fc * BW) / parameters.samplerate);
            float D = 2.0f * std::cos(2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = 1.0f / (1.0f + C);
            coefficients.a1 = 0.0;
            coefficients.a2 = -coefficients.a0;
            coefficients.b1 = -coefficients.a0*(C*D);
            coefficients.b2 = coefficients.a0 * (C - 1.0f);
            coefficients.order = 2;
        }
        case BANDSTOPBUTTERWORTH:
        {
            float BW = parameters.fc / parameters.Q;
            float C =  std::tan((juce::MathConstants<float>::pi * parameters.fc * BW) / parameters.samplerate);
            float D = 2.0f * std::cos(2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = 1.0f / (1.0f + C);
            coefficients.a1 = -coefficients.a0 * D;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -coefficients.a0*D;
            coefficients.b2 = coefficients.a0 * (1.0f- C);
            coefficients.order = 2;
        }
        case AllPASSFFILTER:
        {
            float alpha = (std::tan(juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate) -1) / (std::tan(juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate) +1);
            coefficients.a0 = alpha;
            coefficients.a1 = 1.0f;
            coefficients.a2 = 0.0;
            coefficients.b1 = alpha;
            coefficients.b2 = 0.0;
            coefficients.order = 1;
        }
        case ALPASSFILTER2ndOrder: {
            float BW = parameters.fc / parameters.Q;
            float alpha = (std::tan(((BW * juce::MathConstants<float>::pi) / parameters.samplerate))-1) / (std::tan(((BW * juce::MathConstants<float>::pi) / parameters.samplerate))+1);
            float beta = -std::cos(2.0f * juce::MathConstants<float>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = -alpha;
            coefficients.a1 = beta * (1 - alpha);
            coefficients.a2 = 1.0f;
            coefficients.b1 = coefficients.a1;
            coefficients.b2 = -alpha;
            coefficients.order = 2;
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
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto out = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto inputSample = out[i];
            const auto filterdSample = coefficients.a0 * inputSample + x1State[channel];
            x1State[channel] = coefficients.a1 * inputSample - coefficients.b1 * filterdSample + x2State[channel];
            x2State[channel] = coefficients.a2 * inputSample - coefficients.b2 * filterdSample;
            out[i] = filterdSample;
        }
    }
}


