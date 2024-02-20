/*
  ==============================================================================

    PianoVoice.h
    Created: 15 Jan 2024 8:49:45am
    Author:  Administrator

  ==============================================================================
*/

#pragma once

#include"JuceHeader.h"

struct SnnuPianoVioce : public SamplerVoice
{
    bool canPlaySound(juce::SynthesiserSound* sound) override;

};