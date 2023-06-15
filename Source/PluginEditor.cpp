/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyCompressorEditor::MyCompressorEditor(MyGlueCompressor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    m_attackSlider.setRange(1.0, 100.0);
    m_attackSlider.addListener(this);

        m_releaseSlider.setRange(10.0, 1000.0);
        m_releaseSlider.addListener(this);

        m_ratioSlider.setRange(1.0, 10.0);
        m_ratioSlider.addListener(this);

        m_threshSlider.setRange(-60.0, 0.0);
        m_threshSlider.addListener(this);

        m_makeupGainSlider.setRange( - 24.0, 24.0);
        m_makeupGainSlider.addListener(this);

        m_rangeSlider.setRange(1.0, 40.0);
        m_rangeSlider.addListener(this);

        m_wetDrySlider.setRange(0.0, 1.0);
        m_wetDrySlider.addListener(this);

        m_softClippingToggle.setButtonText("Soft Clipping");
        m_softClippingToggle.addListener(this);

}

MyCompressorEditor::~MyCompressorEditor()
{
}

void MyCompressorEditor::sliderValueChanged(juce::Slider* slider)
{

}

void MyCompressorEditor::buttonClicked(juce::Button* button)
{

}

//==============================================================================
void MyCompressorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MyCompressorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
