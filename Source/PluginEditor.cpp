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
    //g.setGradientFill(
    //    juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).darker(0.05f)
    //    ,getHeight()
    //    ,juce::Colour::fromRGB(202, 222, 220).brighter(0.02f), getHeight() * 0.4));

    ////g.fillEllipse(0.f,0.f,740.f,480.f);
    ////g.fillRect(getLocalBounds());
    //g.fillRoundedRectangle(0.f,0.f, 750.f,480.f,50.f);
}

//void MyCompressorEditor::resized()
//{
//    //// The FlexBox code you provided
//    //auto dialSize = getWidth() * JUCE_LIVE_CONSTANT(0.3);
//
//    //// First column - Attack - release - ratio
//    //juce::FlexBox flexboxColumnOne;
//    //flexboxColumnOne.flexDirection = juce::FlexBox::Direction::column;
//    //flexboxColumnOne.justifyContent = juce::FlexBox::JustifyContent::flexStart;
//    //flexboxColumnOne.alignItems = juce::FlexBox::AlignItems::flexStart;
//
//    //juce::FlexItem attackItem(m_attackDial);
//    //attackItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
//    //flexboxColumnOne.items.add(attackItem);
//
//    //juce::FlexItem releaseItem(m_releaseDial);
//    //releaseItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
//    //flexboxColumnOne.items.add(releaseItem);
//
//    //juce::FlexItem ratioItem(m_ratioDial);
//    //ratioItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
//    //flexboxColumnOne.items.add(ratioItem);
//
//    //// Perform layout
//    //flexboxColumnOne.performLayout(getLocalBounds());
//
//    //// Set the bounds for each slider within the FlexItems
//    //m_attackDial.setBounds(m_attackDial.getX(), m_attackDial.getY(), m_attackDial.getWidth(), dialSize);
//    //m_releaseDial.setBounds(m_releaseDial.getX(), m_releaseDial.getY(), m_releaseDial.getWidth(), dialSize);
//    //m_ratioDial.setBounds(m_ratioDial.getX(), m_ratioDial.getY(), m_ratioDial.getWidth(), dialSize);
//    
//    DBG("Resized called");
//
//}

void MyCompressorEditor::resized()
{
    const int dialSize = getWidth() * 0.3;

    // Left region - Attack, Release, Ratio
    juce::FlexBox flexboxLeft;
    flexboxLeft.flexDirection = juce::FlexBox::Direction::column;
    flexboxLeft.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    flexboxLeft.alignItems = juce::FlexBox::AlignItems::flexStart;

    juce::FlexItem attackItem(m_attackDial);
    attackItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxLeft.items.add(attackItem);

    juce::FlexItem releaseItem(m_releaseDial);
    releaseItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxLeft.items.add(releaseItem);

    juce::FlexItem ratioItem(m_ratioDial);
    ratioItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(0, 10, 10, 10));
    flexboxLeft.items.add(ratioItem);

    flexboxLeft.performLayout(getLocalBounds());

    // Middle region - Threshold and Makeup
    const int middleRegionWidth = getWidth() / 2 - dialSize / 2;

    juce::FlexBox flexboxMiddle;
    flexboxMiddle.flexDirection = juce::FlexBox::Direction::row;
    flexboxMiddle.justifyContent = juce::FlexBox::JustifyContent::center;
    flexboxMiddle.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexItem thresholdItem(m_thresholdDial);
    thresholdItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(10));
    flexboxMiddle.items.add(thresholdItem);

    juce::FlexItem makeupItem(m_makeupGainDial);
    makeupItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(10));
    flexboxMiddle.items.add(makeupItem);

    flexboxMiddle.performLayout(
        juce::Rectangle<int>(middleRegionWidth, 0, dialSize, getHeight()));

    // Right region - Range and Dry/Wet
    juce::FlexBox flexboxRight;
    flexboxRight.flexDirection = juce::FlexBox::Direction::column;
    flexboxRight.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    flexboxRight.alignItems = juce::FlexBox::AlignItems::flexEnd;

    juce::FlexItem rangeItem(m_rangeDial);
    rangeItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(10, 10, 0, 10));
    flexboxRight.items.add(rangeItem);

    juce::FlexItem wetDryItem(m_wetDryDial);
    wetDryItem.withFlex(1.0f).withMargin(juce::FlexItem::Margin(10, 10, 0, 10));
    flexboxRight.items.add(wetDryItem);

    flexboxRight.performLayout(
        juce::Rectangle<int>(getWidth() - dialSize, 0, dialSize, getHeight()));

    // Position text boxes above the parameter dials
    const int textBoxWidth = dialSize;
    const int textBoxHeight = 20;

    m_attackDial.setTextBoxStyle(juce::Slider::TextBoxAbove, false, textBoxWidth, textBoxHeight);
    m_releaseDial.setTextBoxStyle(juce::Slider::TextBoxAbove, false, textBoxWidth, textBoxHeight);
    m_ratioDial.setTextBoxStyle(juce::Slider::TextBoxAbove, false, textBoxWidth, textBoxHeight);

    // Set the bounds for each dial
    m_attackDial.setBounds(m_attackDial.getX(), m_attackDial.getY(), dialSize, dialSize);
    m_releaseDial.setBounds(m_releaseDial.getX(), m_releaseDial.getY(), dialSize, dialSize);
    m_ratioDial.setBounds(m_ratioDial.getX(), m_ratioDial.getY(), dialSize, dialSize);
    m_thresholdDial.setBounds(m_thresholdDial.getX(), m_thresholdDial.getY(), dialSize, dialSize);
    m_makeupGainDial.setBounds(m_makeupGainDial.getX(), m_makeupGainDial.getY(), dialSize, dialSize);
    m_rangeDial.setBounds(m_rangeDial.getX(), m_rangeDial.getY(), dialSize, dialSize);
    m_wetDryDial.setBounds(m_wetDryDial.getX(), m_wetDryDial.getY(), dialSize, dialSize);
}


