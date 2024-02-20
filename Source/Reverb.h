/*
  ==============================================================================

    Reverb.h
    Created: 7 Jan 2024 4:41:35pm
    Author:  Administrator

  ==============================================================================
*/
#include"JuceHeader.h"

#pragma once
class ReverParameters
{
public:
    ReverParameters();
    ~ReverParameters();

    juce::AudioProcessorValueTreeState::ParameterLayout creatParameterLayout();

};