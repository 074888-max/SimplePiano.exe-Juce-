/*
  ==============================================================================

    SnnuPianoSynth.cpp
    Created: 15 Jan 2024 10:40:34am
    Author:  Administrator

  ==============================================================================
*/

#include "SnnuPianoSynth.h"

SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState):keyboardState(keyState)
{
    {
        formatmanager.registerBasicFormats();

        //std::unique_ptr<AudioFormatReader> pianoC(formatmanager.createReaderFor(
        //    juce::File("C:\\Program Files (x86)\\SnnuMidiPiano\\PianoC4.wav")
        //));

        std::unique_ptr<juce::MemoryInputStream> in_stream(new juce::MemoryInputStream(BinaryData::PianoC4_wav, BinaryData::PianoC4_wavSize,false));
        std::unique_ptr<juce::AudioFormatReader> pianoC(formatmanager.createReaderFor(std::move(in_stream)));

        //std::unique_ptr<juce::MemoryInputStream> in_stream = std::make_unique<juce::MemoryInputStream>(BinaryData::PianoC4_wav, BinaryData::PianoC4_wavSize,false);
        //std::unique_ptr<juce::AudioFormatReader> pianoC = std::make_unique<juce::AudioFormatReader>(formatmanager.createReaderFor(std::move(in_stream))); //this method is not correct ,because audioFormatReader is subClass


        allNotes.setRange(0, 127, true);

        for (auto i = 0; i < 10; ++i)               
            synth.addVoice(new SnnuPianoVioce());

        synth.addSound(new SnnuPianoSound("sample1",
            *pianoC,
            allNotes,
            60,
            0.01,
            3,
            10.0
        ));

    }
}

void SynthAudioSource::setUsingSineWaveSound()
{

    synth.clearSounds();

}

void SynthAudioSource::prepareToPlay(int, double sampleRate)
{
    
    synth.setCurrentPlaybackSampleRate(sampleRate); 
    allMidiKey.reset(sampleRate);
    
}

void SynthAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::MidiBuffer incomingMidi;
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
        bufferToFill.numSamples, true);                                          //把incomingMidi弄成keyComponent里面的输入

    allMidiKey.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples); //把incomingMidi弄成你按的midi  之前已经device.addmidicallback这个了所以接受了来自你的midi

    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
        bufferToFill.startSample, bufferToFill.numSamples); //发声samplerVoice和samplerSounds
}
