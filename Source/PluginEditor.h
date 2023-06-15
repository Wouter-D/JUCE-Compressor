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

    juce::ToggleButton m_softClippingToggle;

    juce::Slider m_inputDial;
    juce::Slider m_attackDial;
    juce::Slider m_releaseDial;
    juce::Slider m_ratioDial;
    juce::Slider m_thresholdDial;
    juce::Slider m_makeupGainDial;
    juce::Slider m_rangeDial;
    juce::Slider m_outputDial;
    juce::Slider m_wetDryDial;

    std::vector<juce::Slider*> m_dials =
    {
        &m_inputDial, 
        &m_attackDial,
        &m_releaseDial,
        &m_ratioDial,
        &m_thresholdDial,
        &m_makeupGainDial,
        &m_rangeDial,
        &m_outputDial,
        &m_wetDryDial
    };

    juce::Label inputDialLabel;
    juce::Label threshDialLabel;
    juce::Label ratioDialLabel;
    juce::Label attackDialLabel;
    juce::Label releaseDialLabel;
    juce::Label outputDialLabel;
    juce::Label rangeDialLabel;
    juce::Label wetDryDialLabel;
    
    std::vector<juce::Label*> dialLabels =
    {
        &inputDialLabel,
        &threshDialLabel,
        &ratioDialLabel,
        &attackDialLabel,
        &releaseDialLabel,
        &outputDialLabel,
        &rangeDialLabel,
        &wetDryDialLabel
    };

    void setCommonSliderProperties(juce::Slider& slider); 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyCompressorEditor)
};
