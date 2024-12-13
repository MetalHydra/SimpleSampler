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

template <typename T>
struct FilterParameters<T, typename std::enable_if<std::is_same<T, float>::value || std::is_same<T, double>::value>::type>
{
    T fc = 1000.0f;
    T Q = 0.707f;
    T samplerate = 44100.0f;
    FilterType filterType = LOWPASS2ndOrder;
};

template <typename T>
struct filterCoefficients<T, typename std::enable_if<std::is_same<T, float>::value || std::is_same<T, double>::value>::type>
{
    T a0 = 0.0f;
    T a1 = 0.0f;
    T a2 = 0.0f;
    T b1 = 0.0f;
    T b2 = 0.0f;
    T c0 = 1.0f;
    T d0 = 0.0f;
    int order = 1;
};

template <typename T>
class BiQuadFilter<T, typename std::enable_if<std::is_same<T, float>::value || std::is_same<T, double>::value>::type>
{
    public:
        BiQuadFilter() = default;
        BiQuadFilter(filterCoefficients<T> coefficients) : coefficients(coefficients) {};
        ~BiQuadFilter() = default;

        void updateParameters(T cutoff, T Q, T samplerate, FilterType filterType)
        {
            parameters.fc = cutoff;
            parameters.Q = Q;
            parameters.samplerate = samplerate;
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
                    out[i] = filteredSample;
                }
            }
        }

        void calculateFirstOrderLowPass()
        {
            T theta = 2.0f * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            T gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1.0f - gamma) / 2.0f;
            coefficients.a1 = coefficients.a0;
            coefficients.a2 = 0.0f;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            coefficients.c0 = 1.0f;
            coefficients.d0 = 0.0f;
            coefficients.order = 1;
        }

        void calculateFirstOrderHighPass()
        {
            T theta = 2.0f * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            T gamma = std::cos(theta) / (1 + std::sin(theta));
            coefficients.a0 = (1.0f + gamma) / 2.0f;
            coefficients.a1 = -coefficients.a0;
            coefficients.a2 = 0.0f;
            coefficients.b1 = -gamma;
            coefficients.b2 = 0.0f;
            coefficients.c0 = 1.0f;
            coefficients.d0 = 0.0f;
            coefficients.order = 1;
        }

        void calculateSecondOrderLowPass()
        {
            auto theta = 2.0f * juce::MathConstants<T>::pi * parameters.fc / parameters.samplerate;
            auto d = 1.0f / parameters.Q;
            auto num = 1.0f - (d/2.0f) * std::sin(theta);
            auto denum = 1.0f + (d/2.0f) * std::sin(theta);
            auto beta = 0.5f * (num / denum);
            auto gamma = (0.5f + beta) * std::cos(theta);
            coefficients.a0 = (0.5f + beta - gamma) / 2.0f;
            coefficients.a1 = 0.5f + beta - gamma;
            coefficients.a2 = coefficients.a0;
            coefficients.b1 = -2.0f *gamma;
            coefficients.b2 = 2.0f * beta;
            coefficients.order = 2;
        }

        void reset()
        {
            x1State = {0.0f, 0.0f};
            x2State = {0.0f, 0.0f};
            y1State = {0.0f, 0.0f};
            y2State = {0.0f, 0.0f};
        }

    private:
        filterCoefficients<T> coefficients;
        FilterParameters<T> parameters;
        std::vector<T> x1State = {0.0f, 0.0f};
        std::vector<T> x2State = {0.0f, 0.0f};
        std::vector<T> y1State = {0.0f, 0.0f};
        std::vector<T> y2State = {0.0f, 0.0f};

        JUCE_LEAK_DETECTOR(BiQuadFilter)
};
