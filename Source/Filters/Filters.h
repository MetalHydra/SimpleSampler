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

// gain should be applied in db (10^(gain/20))
template <typename T>
struct FilterParameters
{
    std::atomic<T> fc = 1000.0;
    std::atomic<T> Q = 0.707;
    std::atomic<T> gain = 1.0;
    std::atomic<T> samplerate = 44100.0;
    FilterType filterType = LOWPASS2ndOrder;
};

template <typename T>
struct filterCoefficients
{
    std::atomic<T> a0 = 0.0;
    std::atomic<T> a1 = 0.0;
    std::atomic<T> a2 = 0.0;
    std::atomic<T> b1 = 0.0;
    std::atomic<T> b2 = 0.0;
    std::atomic<T> c0 = 1.0;
    std::atomic<T> d0 = 0.0;
    std::atomic<int> order = 1;
};

template <typename T>
class BiQuadFilter
{
    public:
        BiQuadFilter() = default;
        BiQuadFilter(filterCoefficients<T> coefficients) : coefficients(coefficients) {};
        ~BiQuadFilter() = default;

        void updateParameters(T cutoff, T Q, T samplerate, T gain, FilterType filterType)
        {
            parameters.fc = cutoff;
            parameters.Q = Q;
            parameters.samplerate = samplerate;
            parameters.gain = gain;
            parameters.filterType = filterType;
        }

        filterCoefficients<T>& getFilterParameters() { return coefficients; }

        void ProcessBlock(juce::AudioBuffer<T>& buffer)
        {
            for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto out = buffer.getWritePointer(channel);
                for (auto i = 0; i < buffer.getNumSamples(); ++i)
                {
                    const auto inputSample = out[i];
                    const auto filteredSample = coefficients.a0 * inputSample + x1State[channel];
                    x1State[channel] = coefficients.a1 * inputSample - coefficients.b1 * filteredSample + x2State[channel];
                    x2State[channel] = coefficients.a2 * inputSample - coefficients.b2 * filteredSample;
                    out[i] = coefficients.d0 * inputSample + coefficients.c0 * filteredSample;
                }
            }
        }

        void calculateFirstOrderLPF()
        {
            T theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            T gamma = std::cos(theta) / (1.0 + std::sin(theta));
            coefficients.a0 = (1.0 - gamma) / 2.0;
            coefficients.a1 = coefficients.a0;
            coefficients.a2 = 0.0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0;
            coefficients.c0 = 1.0;
            coefficients.d0 = 0.0;
            coefficients.order = 1;
        }

        void calculateFirstOrderHPF()
        {
            T theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            T gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1.0 + gamma) / 2.0;
            coefficients.a1 = -coefficients.a0;
            coefficients.a2 = 0.0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0;
            coefficients.c0 = 1.0;
            coefficients.d0 = 0.0;
            coefficients.order = 1;
        }

        void calculateSecondOrderLPF()
        {
            auto theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            auto d = 1.0 / parameters.Q;
            auto num = 1.0 - (d/2.0) * std::sin(theta);
            auto denum = 1.0 + (d/2.0) * std::sin(theta);
            auto beta = 0.5 * (num / denum);
            auto gamma = (0.5 + beta) * std::cos(theta);
            coefficients.a0 = (0.5 + beta - gamma) / 2.0;
            coefficients.a1 = 0.5 + beta - gamma;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -2.0 *gamma;
            coefficients.b2 = 2.0 * beta;
            coefficients.order = 2;
        }

        void calculateSecondOrderHPF()
        {
            auto theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            auto d = 1.0 / parameters.Q;
            auto num = 1.0 - (d/2.0) * std::sin(theta);
            auto denum = 1.0 + (d/2.0) * std::sin(theta);
            auto beta = 0.5 * (num / denum);
            auto gamma = (0.5 + beta) * std::cos(theta);
            coefficients.a0 = (0.5f + beta + gamma) / 2.0;
            coefficients.a1 = 0.5f + beta + gamma;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -2.0 *gamma;
            coefficients.b2 = 2.0 * beta;
            coefficients.order = 2;
        }

        void calculating2ndOrderBPF()
        {
            auto K = std::tan((juce::MathConstants<T>::pi * parameters.fc) / parameters.samplerate);
            auto delta = (K*K*parameters.Q) + K + parameters.Q;
            coefficients.a0 = K / delta;
            coefficients.a1 = 0.0;
            coefficients.a2 = -K/delta;
            coefficients.b1 = ((2.0 * parameters.Q)*(K*K-1)) / delta;
            coefficients.b2 = ((K*K*parameters.Q)-K+parameters.Q) / delta;
            coefficients.order = 2;
        }

        void calculating2ndOrderBSF()
        {
            auto  K = std::tan((juce::MathConstants<T>::pi * parameters.fc) / parameters.samplerate);
            auto delta = (K*K*parameters.Q) + K + parameters.Q;
            coefficients.a0 = parameters.Q*(K*K+1.0) / delta;
            coefficients.a1 = 2*parameters.Q*(K*K-1.0) / delta;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = coefficients.a1;
            coefficients.b2 = ((K*K*parameters.Q)-K+parameters.Q) / delta;
            coefficients.order = 2;
        }

        void calculate2ndOrderButterworthLPF()
        {
            auto C = 1.0f /  std::tan((juce::MathConstants<T>::pi * parameters.fc) / parameters.samplerate);
            coefficients.a0 = 1.0 / (1.0 + std::sqrt(2*C) + C*C);
            coefficients.a1 = 2.0f * coefficients.a0;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = 2.0 * coefficients.a0 * (1.0 - C*C);
            coefficients.b2 = coefficients.a0 * (1.0 - std::sqrt(2*C) + C*C);
            coefficients.order = 2;
        }

        void calculate2ndOrderButterworthHPF()
        {
            auto C = 1.0f /  std::tan((juce::MathConstants<T>::pi * parameters.fc) / parameters.samplerate);
            coefficients.a0 = 1.0 / (1.0 + std::sqrt(2*C) + C*C);
            coefficients.a1 = -2.0f * coefficients.a0;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = 2.0f * coefficients.a0 * (C*C - 1.0);
            coefficients.b2 = coefficients.a0 * (1.0 - std::sqrt(2*C) + C*C);
            coefficients.order = 2;
        }

        void calculateButterworthBPF()
        {
            auto BW = parameters.fc / parameters.Q;
            auto C = 1 / std::tan((juce::MathConstants<T>::pi * parameters.fc * BW) / parameters.samplerate);
            auto D = 2.0 * std::cos(2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = 1.0 / (1.0+ C);
            coefficients.a1 = 0.0;
            coefficients.a2 = -coefficients.a0;
            coefficients.b1 = -coefficients.a0*(C*D);
            coefficients.b2 = coefficients.a0 * (C - 1.0);
            coefficients.order = 2;
        }

        void calculate2ndOrderButterworthBSF()
        {
            auto BW = parameters.fc / parameters.Q;
            auto C =  std::tan((juce::MathConstants<T>::pi * parameters.fc * BW) / parameters.samplerate);
            auto D = 2.0 * std::cos(2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = 1.0f / (1.0 + C);
            coefficients.a1 = -coefficients.a0 * D;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -coefficients.a0*D;
            coefficients.b2 = coefficients.a0 * (1.0 - C);
            coefficients.order = 2;
        }

        void calculateAPF()
        {
            auto alpha = (std::tan(juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate) -1) / (std::tan(juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate) +1);
            coefficients.a0 = alpha;
            coefficients.a1 = 1.0;
            coefficients.a2 = 0.0;
            coefficients.b1 = alpha;
            coefficients.b2 = 0.0;
            coefficients.order = 1;
        }

        void calculate2ndOrderAPF()
        {
            auto BW = parameters.fc / parameters.Q;
            auto alpha = (std::tan(((BW * juce::MathConstants<T>::pi) / parameters.samplerate))-1) / (std::tan(((BW * juce::MathConstants<T>::pi) / parameters.samplerate))+1);
            auto beta = -std::cos(2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate);
            coefficients.a0 = -alpha;
            coefficients.a1 = beta * (1.0 - alpha);
            coefficients.a2 = 1.0;
            coefficients.b1 = coefficients.a1;
            coefficients.b2 = -alpha;
            coefficients.order = 2;
        }

        void calculateLowShelf()
        {
            auto theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            auto mu = parameters.gain;
            auto tmp = 1.0 + mu;
            auto beta = 4.0 / tmp;
            auto delta = beta * std::tan(theta*0.5);
            auto gamma = (1.0 - delta) / (1.0 + delta);
            coefficients.a0 = (1.0 - gamma) * 0.5;
            coefficients.a1 = coefficients.a0;
            coefficients.a2 = 0.0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0;
            coefficients.c0 = mu - 1.0;
            coefficients.d0 = 1.0;
            coefficients.order = 1;
        }

        void calculateHighShelf()
        {
            auto theta = 2.0 * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            auto mu = parameters.gain;
            auto tmp = 1.0 + mu;
            auto beta = tmp * 0.25;
            auto delta = beta * std::tan(theta * 0.5);
            auto gamma = (1.0 - delta) / (1.0 + delta);
            coefficients.a0 = (1.0 + gamma) * 0.5;
            coefficients.a1 = -coefficients.a0;
            coefficients.a2 = 0.0;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0;
            coefficients.c0 = mu - 1.0;
            coefficients.d0 = 1.0;
            coefficients.order = 1;
        }

        void reset()
        {
            x1State = {0.0, 0.0};
            x2State = {0.0, 0.0};
            y1State = {0.0, 0.0};
            y2State = {0.0, 0.0};
        }

        void toggleBypass(bool currentState)
        {
            bypass = !currentState;
        }

    private:
        filterCoefficients<T> coefficients;
        FilterParameters<T> parameters;
        std::vector<T> x1State = {0.0, 0.0};
        std::vector<T> x2State = {0.0, 0.0};
        std::vector<T> y1State = {0.0, 0.0};
        std::vector<T> y2State = {0.0, 0.0};
        std::atomic<bool> bypass = false;

        JUCE_LEAK_DETECTOR(BiQuadFilter)
};
