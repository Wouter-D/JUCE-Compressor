/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MyCompressorEditor : public juce::AudioProcessorEditor,
    public juce::Slider::Listener,
    public juce::Button::Listener
{
public:
    explicit MyCompressorEditor(MyGlueCompressor& p);
    ~MyCompressorEditor() override;

    //==============================================================================
    void sliderValueChanged(juce::Slider* slider);
    void buttonClicked(juce::Button* button);
    
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    MyGlueCompressor& audioProcessor;

    //juce::Slider m_inputSlider;
    juce::Slider m_attackSlider;
    juce::Slider m_releaseSlider;
    juce::Slider m_ratioSlider;
    juce::Slider m_threshSlider;
    juce::Slider m_makeupGainSlider;
    juce::Slider m_rangeSlider;
    //juce::Slider m_outputSlider;
    juce::Slider m_wetDrySlider;
    juce::ToggleButton m_softClippingToggle;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyCompressorEditor)
};
