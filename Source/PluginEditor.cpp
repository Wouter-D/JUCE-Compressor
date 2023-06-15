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
    g.setGradientFill(
        juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).darker(0.05f)
        ,getHeight()
        ,juce::Colour::fromRGB(202, 222, 220).brighter(0.02f), getHeight() * 0.4));

    //g.fillEllipse(0.f,0.f,740.f,480.f);
    //g.fillRect(getLocalBounds());
    g.fillRoundedRectangle(0.f,0.f, 750.f,480.f,50.f);
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
    auto halfWidth = getWidth() / 2;
    auto quarterWidth = getWidth() / 4;
    auto dialSize = quarterWidth * JUCE_LIVE_CONSTANT(0.3);

    juce::FlexBox flexbox;
    flexbox.flexDirection = juce::FlexBox::Direction::row;
    flexbox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexbox.alignItems = juce::FlexBox::AlignItems::center;

    // First region (left panel) - Attack, Release, Ratio
    juce::FlexBox flexboxLeft;
    flexboxLeft.flexDirection = juce::FlexBox::Direction::column;
    flexboxLeft.justifyContent = juce::FlexBox::JustifyContent::center;
    flexboxLeft.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexItem attackItem(m_attackDial);
    attackItem.withMargin(juce::FlexItem::Margin(0, 0, 10, 0));
    flexboxLeft.items.add(attackItem);

    juce::FlexItem releaseItem(m_releaseDial);
    releaseItem.withMargin(juce::FlexItem::Margin(0, 0, 10, 0));
    flexboxLeft.items.add(releaseItem);

    juce::FlexItem ratioItem(m_ratioDial);
    flexboxLeft.items.add(ratioItem);

    flexbox.items.add(flexboxLeft);

    // Second region (middle panel) - Threshold, Makeup
    juce::FlexBox flexboxMiddle;
    flexboxMiddle.flexDirection = juce::FlexBox::Direction::row;
    flexboxMiddle.justifyContent = juce::FlexBox::JustifyContent::center;
    flexboxMiddle.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexItem thresholdItem(m_thresholdDial);
    thresholdItem.withMargin(juce::FlexItem::Margin(10, 0, 10, 0));
    flexboxMiddle.items.add(thresholdItem);

    juce::FlexItem makeupItem(m_makeupGainDial);
    flexboxMiddle.items.add(makeupItem);

    flexbox.items.add(flexboxMiddle);

    // Third region (right panel) - Range, Dry/Wet
    juce::FlexBox flexboxRight;
    flexboxRight.flexDirection = juce::FlexBox::Direction::column;
    flexboxRight.justifyContent = juce::FlexBox::JustifyContent::center;
    flexboxRight.alignItems = juce::FlexBox::AlignItems::center;

    juce::FlexItem rangeItem(m_rangeDial);
    rangeItem.withMargin(juce::FlexItem::Margin(0, 0, 10, 0));
    flexboxRight.items.add(rangeItem);

    juce::FlexItem wetDryItem(m_outputDial);
    flexboxRight.items.add(wetDryItem);

    flexbox.items.add(flexboxRight);

    // Perform layout
    flexbox.performLayout(getLocalBounds());

    // Set the bounds for each slider within the FlexItems
    m_attackDial.setBounds(m_attackDial.getX(), m_attackDial.getY(), m_attackDial.getWidth(), dialSize);
    m_releaseDial.setBounds(m_releaseDial.getX(), m_releaseDial.getY(), m_releaseDial.getWidth(), dialSize);
    m_ratioDial.setBounds(m_ratioDial.getX(), m_ratioDial.getY(), m_ratioDial.getWidth(), dialSize);
    m_thresholdDial.setBounds(m_thresholdDial.getX(), m_thresholdDial.getY(), dialSize, m_thresholdDial.getHeight());
    m_makeupGainDial.setBounds(m_makeupGainDial.getX(), m_makeupGainDial.getY(), dialSize, m_makeupGainDial.getHeight());
    m_rangeDial.setBounds(m_rangeDial.getX(), m_rangeDial.getY(), m_rangeDial.getWidth(), dialSize);
    m_outputDial.setBounds(m_outputDial.getX(), m_outputDial.getY(), m_outputDial.getWidth(), dialSize);

    
    // Update the size and position of the toggle button
    m_softClippingToggle.setBounds(halfWidth, getHeight() - 40, halfWidth, 30);
}

