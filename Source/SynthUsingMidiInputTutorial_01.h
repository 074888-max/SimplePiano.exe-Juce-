#include"Reverb.h"
#include"SnnuPianoSynth.h"

float globalVolume;

#pragma once

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent,
                               private juce::Timer,
                               public juce::KeyListener
{
public:
    MainContentComponent()
        : synthAudioSource  (keyboardState),
          keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
    {
        addAndMakeVisible (keyboardComponent);
        setAudioChannels (0, 2);

        setSize (512*2, 512*2);
        startTimer (400);

        addKeyListener(this);
        setWantsKeyboardFocus(true);

        {

            addAndMakeVisible(midiInputListLabel);
            midiInputListLabel.setText("MIDI Input:", juce::dontSendNotification);
            midiInputListLabel.attachToComponent(&midiInputList, true);

            auto midiInputs = juce::MidiInput::getAvailableDevices();
            addAndMakeVisible(midiInputList);
            midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
            midiInputList.setColour(juce::ComboBox::backgroundColourId, juce::Colours::cadetblue.withAlpha(0.5f));
            midiInputList.setColour(juce::ComboBox::textColourId, juce::Colours::black);
            midiInputList.setColour(juce::ComboBox::arrowColourId, juce::Colours::transparentBlack);
            midiInputList.setColour(juce::ComboBox::buttonColourId, juce::Colours::red);
            midiInputList.setColour(juce::ComboBox::focusedOutlineColourId, juce::Colours::red);

            juce::StringArray midiInputNames;

            for (auto& input : midiInputs)
                midiInputNames.add(input.name);

            midiInputList.addItemList(midiInputNames, 1);
            midiInputList.onChange = [this] { 
                setMidiInput(midiInputList.getSelectedId());
                };

            for (auto& input : midiInputs)
            {
                if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
                {
                    setMidiInput(midiInputs.indexOf(input));
                    break;
                }
            }

            if (midiInputList.getSelectedId() == 0)
                setMidiInput(0);
        }

        keyboardComponent.setBounds(getWidth() * 0.25, getHeight() * 0.9, getWidth() * 0.5, getHeight() * 0.1);
        midiInputList.setBounds(getWidth() * 0.5 - midiInputList.getWidth() * 0.5, 15, getWidth() * 0.2, 25);

        synthPic = ImageCache::getFromMemory(BinaryData::MidiPiano_1_png,BinaryData::MidiPiano_1_pngSize);

        addAndMakeVisible(volumeSlider);
        volumeSlider.setBounds(getWidth() * 0.05, getHeight() * 0.45, 50, 150);
        volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
        volumeSlider.setRange(0, 100, 1);
        volumeSlider.setColour(juce::Slider::trackColourId, juce::Colours::skyblue.withAlpha(0.5f));
        volumeSlider.onValueChange = [this]() {globalVolume = volumeSlider.getValue();};
        volumeLabel.setText("Volume:",juce::dontSendNotification);
        volumeLabel.attachToComponent(&volumeSlider,false);

        addAndMakeVisible(reverbSlider);
        reverbSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        reverbSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke);
        reverbSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::cadetblue.withAlpha(0.3f));
        reverbSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::bisque);
        reverbSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 30);
        reverbSlider.setTextValueSuffix("Reverb");
        reverbSlider.setBounds(getWidth() * 0.85, getHeight() * 0.45, 150, 150);
        reverbSlider.setRange(0, 1, 0.01);
        /*reverbSlider.onValueChange = [this]() {};*/



    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
        keyboardComponent.setBounds(getWidth() * 0.25, getHeight() * 0.9, getWidth() * 0.5, getHeight() * 0.1);
        midiInputList.setBounds(getWidth() * 0.5 - midiInputList.getWidth() * 0.5, 15, getWidth() * 0.2, 25);

        volumeSlider.setBounds(getWidth() * 0.05, getHeight() * 0.45, 25, 150);

        reverbSlider.setBounds(getWidth() * 0.85, getHeight() * 0.45, 150, 150);
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlockExpected;
        spec.numChannels = 1;

        leftReverb.prepare(spec);
        rightReverb.prepare(spec);
    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        synthAudioSource.getNextAudioBlock (bufferToFill);

        reverbParameter.roomSize = reverbSlider.getValue();
        leftReverb.setParameters(reverbParameter);
        rightReverb.setParameters(reverbParameter);

        
        juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

        juce::dsp::ProcessContextReplacing<float> leftContext(block.getSingleChannelBlock(0));
        juce::dsp::ProcessContextReplacing<float> rightContext(block.getSingleChannelBlock(1));

        leftReverb.process(leftContext);
        rightReverb.process(rightContext);

        processVolume(bufferToFill.buffer, bufferToFill.numSamples, 0, bufferToFill.startSample);
        processVolume(bufferToFill.buffer, bufferToFill.numSamples, 1, bufferToFill.startSample);

    }

    void releaseResources() override
    {
        synthAudioSource.releaseResources();
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::ghostwhite);
        g.fillAll();

        g.drawImage(synthPic,0, 0, getWidth(), getHeight(),
            0, 0, synthPic.getHeight(),synthPic.getWidth());
    }

    void setMidiInput(int index)
    {
        auto list = juce::MidiInput::getAvailableDevices(); 

        deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier,
            synthAudioSource.getMidiCollector()); // [12]

        auto newInput = list[index-1];

        if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
            deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

        deviceManager.addMidiInputDeviceCallback(newInput.identifier, synthAudioSource.getMidiCollector()); //adjust incomingMidi to
        deviceManager.addMidiInputDeviceCallback(newInput.identifier, &synthAudioSource);  //update keycompoent ui
        midiInputList.setSelectedId(index , juce::dontSendNotification);

        lastInputIndex = index;

    }

    bool keyPressed(const KeyPress& key, Component* originatingComponent) override
    {

        return 1;
    }

    void processVolume(juce::AudioBuffer<float>* sourceVolumePtr,int num,int channel,int starSample)
    {
        auto volume = volumeSlider.getValue() / 10;

        auto outBuffer = sourceVolumePtr->getWritePointer(channel);

        for (size_t lo = 0; lo < num; lo++)
        {
            outBuffer[lo] *= volume;
        }

    }
private:
    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }

    //==========================================================================
    juce::MidiKeyboardState keyboardState;
    SynthAudioSource synthAudioSource;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;

    juce::Image synthPic;

    juce::Slider volumeSlider;
    juce::Label volumeLabel;

    juce::dsp::Reverb leftReverb, rightReverb;
    juce::dsp::Reverb::Parameters reverbParameter;
    juce::Slider reverbSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
