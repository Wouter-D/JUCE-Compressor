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
    //Go tru each slider and call dial properties
    for (size_t index = 0; index < m_dials.size(); index++)
    {
        setCommonSliderProperties(*m_dials[index]);
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(750,480);
    
    m_attackDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_attackDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    m_attackDial.setRange(1.0, 100.0);
    m_attackDial.setValue(50.0);

    m_releaseDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_releaseDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    m_releaseDial.setRange(10.0, 1000.0);
    m_releaseDial.setValue(500.0);

    m_ratioDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    m_ratioDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    m_ratioDial.setRange(1.0, 10.0);
    m_ratioDial.setValue(5.0);
}

MyCompressorEditor::~MyCompressorEditor()
{
    m_dials.clear();
    //Shrinks to only the needed elements in the vector
    m_dials.shrink_to_fit();
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
    g.setGradientFill(
        juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).darker(0.05f)
        ,getHeight()
        ,juce::Colour::fromRGB(202, 222, 220).brighter(0.02f), getHeight() * 0.4));

    //g.fillEllipse(0.f,0.f,740.f,480.f);
    //g.fillRect(getLocalBounds());
    g.fillRoundedRectangle(0.f,0.f, 750.f,480.f,50.f);
}

void MyCompressorEditor::resized()
{
    // The FlexBox code you provided
    auto dialSize = getWidth() * JUCE_LIVE_CONSTANT(0.1);

    // First column - Attack - release - ratio
    juce::FlexBox flexboxColumnOne;
    flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
    flexboxColumnOne.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    flexboxColumnOne.alignItems = juce::FlexBox::AlignItems::flexStart;

    juce::FlexItem attackItem(m_attackDial);
    attackItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxColumnOne.items.add(attackItem);

    juce::FlexItem releaseItem(m_releaseDial);
    releaseItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxColumnOne.items.add(releaseItem);

    juce::FlexItem ratioItem(m_ratioDial);
    ratioItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxColumnOne.items.add(ratioItem);

    // Perform layout
    flexboxColumnOne.performLayout(getLocalBounds());

    // Set the bounds for each slider within the FlexItems
    m_attackDial.setBounds(m_attackDial.getX(), m_attackDial.getY(), m_attackDial.getWidth(), dialSize);
    m_releaseDial.setBounds(m_releaseDial.getX(), m_releaseDial.getY(), m_releaseDial.getWidth(), dialSize);
    m_ratioDial.setBounds(m_ratioDial.getX(), m_ratioDial.getY(), m_ratioDial.getWidth(), dialSize);
}
