/*
  ==============================================================================

    SnnuPianoSynth.h
    Created: 15 Jan 2024 10:40:34am
    Author:  Administrator

  ==============================================================================
*/

#pragma once
#include"JuceHeader.h"
#include"PianoSound.h"
#include"PianoVoice.h"

class SynthAudioSource : public juce::AudioSource,
    public juce::MidiInputCallback
{
public:
    SynthAudioSource(juce::MidiKeyboardState& keyState);

    void setUsingSineWaveSound();

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

    void releaseResources() override {};

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override; //这个函数仅仅传入buffer 然后电脑疯狂调用keyboardState 和 allMidi 检测midi输入


    juce::MidiMessageCollector* getMidiCollector()
    {
        return &allMidiKey;
    }

    virtual void handleIncomingMidiMessage(MidiInput* source,
        const MidiMessage& message)
    {
        keyboardState.processNextMidiEvent(message);
    }

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;

    MidiMessageCollector allMidiKey;

    juce::AudioFormatManager formatmanager;
    BigInteger allNotes;
};