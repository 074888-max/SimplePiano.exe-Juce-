/*
  ==============================================================================

    Reverb.cpp
    Created: 7 Jan 2024 4:41:35pm
    Author:  Administrator

  ==============================================================================
*/

#include "Reverb.h"

ReverParameters::ReverParameters()
{
}

ReverParameters::~ReverParameters()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ReverParameters::creatParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "RoomSize", "RoomSize",
        juce::NormalisableRange<float>(0,1),0.5
    ));

    return layout;
}
