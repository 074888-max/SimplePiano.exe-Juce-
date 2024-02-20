/*
  ==============================================================================

    PianoSound.h
    Created: 15 Jan 2024 8:49:11am
    Author:  Administrator

  ==============================================================================
*/

#pragma once
#include"JuceHeader.h"

struct SnnuPianoSound : public SamplerSound
{
    
    SnnuPianoSound(const String& name,
        AudioFormatReader& source,
        const BigInteger& midiNotes,
        int midiNoteForNormalPitch,
        double attackTimeSecs,
        double releaseTimeSecs,
        double maxSampleLengthSeconds);

};

class MidiMessageCollector_k : public MidiMessageCollector
{
public:
    MidiMessageCollector_k(MidiKeyboardState*  keyState_in);
    void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) override;

private:
    MidiKeyboardState* keyState;
};