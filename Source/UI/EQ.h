//
// Created by dennis on 08.12.24.
//
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Filters/Filters.h"
#include "FilterComponent.h"

class EQ : public juce::Component {
    public:
        EQ(SimpleSamplerAudioProcessor& p);
        ~EQ() override;
        void paint(juce::Graphics &g) override;
        void resized() override;
    private:
        SimpleSamplerAudioProcessor& audioProcessor;
        int numBands = 6;
        FilterComponent filterBand1 = FilterComponent(audioProcessor, "Band1");
        FilterComponent filterBand2 = FilterComponent(audioProcessor, "Band2");
        FilterComponent filterBand3 = FilterComponent(audioProcessor, "Band3");
        FilterComponent filterBand4 = FilterComponent(audioProcessor, "Band4");
        FilterComponent filterBand5 = FilterComponent(audioProcessor, "Band5");
            //, filterBand2, filterBand3, filterBand4, filterBand5, filterBand6;
        std::vector<float> centerFrequncies = { 50, 100, 500, 1000, 5000, 10000 };
        juce::FlexBox eqBox;
};


