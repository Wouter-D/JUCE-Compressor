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

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        // Customize the appearance of the rotary slider
        // You can use the Graphics object (g) to draw custom graphics

        // Example code to draw a basic rotary slider
        g.setColour(juce::Colours::grey);
        g.fillEllipse(x, y, width, height);

        g.setColour(juce::Colours::red);
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        juce::Path trianglePath;
        float triangleSize = std::min(width, height) * 0.4f;
        trianglePath.addTriangle(juce::Point<float>(x + width / 2, y + height / 2),
            juce::Point<float>(x + width / 2 + triangleSize / 2, y + height / 2),
            juce::Point<float>(x + width / 2 + triangleSize / 2 * std::cos(angle),
                y + height / 2 + triangleSize / 2 * std::sin(angle)));
        g.fillPath(trianglePath);
    }
};


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
    juce::LookAndFeel_V4 m_lnf;
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
