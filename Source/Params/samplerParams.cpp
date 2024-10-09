#include "samplerParams.h"
using namespace nSamplerSound;
void nSamplerSound::SamplerParams::setADSR(float attackToUse, float decayToUse, float sustainToUse, float releaseToUse)
{
    adsrParams.attack = attackToUse;
    adsrParams.decay = decayToUse;
    adsrParams.sustain = sustainToUse;
    adsrParams.release = releaseToUse;
}

void nSamplerSound::SamplerParams::setReverb(float roomToUse, float dampToUse, float widthToUse, float wetToUse, float dryToUse)
{
    reverbParams.roomSize = roomToUse;
    reverbParams.damping = dampToUse;
    reverbParams.width = widthToUse;
    reverbParams.wetLevel = wetToUse;
    reverbParams.dryLevel = dryToUse;
    reverbParams.freezeMode = 0.0f;

}