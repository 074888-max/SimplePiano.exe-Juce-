/*
  ==============================================================================

    PianoSound.cpp
    Created: 15 Jan 2024 8:49:11am
    Author:  Administrator
    const String& name,
                  AudioFormatReader& source,
                  const BigInteger& midiNotes,
                  int midiNoteForNormalPitch,
                  double attackTimeSecs,
                  double releaseTimeSecs,
                  double maxSampleLengthSeconds
  ==============================================================================
*/

#include "PianoSound.h"

SnnuPianoSound::SnnuPianoSound(const String& name,
    AudioFormatReader& source,
    const BigInteger& midiNotes,
    int midiNoteForNormalPitch,
    double attackTimeSecs,
    double releaseTimeSecs,
    double maxSampleLengthSeconds):SamplerSound(name,source,midiNotes,midiNoteForNormalPitch,attackTimeSecs,releaseTimeSecs,maxSampleLengthSeconds)
{

}




MidiMessageCollector_k::MidiMessageCollector_k(MidiKeyboardState* keyState_in):keyState(keyState_in)
{
}

void MidiMessageCollector_k::handleIncomingMidiMessage(MidiInput*, const MidiMessage&)
{

}
