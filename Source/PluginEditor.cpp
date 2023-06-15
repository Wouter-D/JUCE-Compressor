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
    ,m_lnf()
    ,inputDialLabel("Input","Input")
    ,threshDialLabel("Threshold", "Threshold")
    ,ratioDialLabel("Ratio", "Ratio")
    ,attackDialLabel("Attack", "Attack")
    ,releaseDialLabel("Release", "Release")
    ,outputDialLabel("Output", "Output")
    ,rangeDialLabel("Range", "Range")
    ,wetDryDialLabel("WetDry", "WetDry")
    ,m_inputDial()
    ,m_attackDial()
    ,m_releaseDial()
    ,m_ratioDial()
    ,m_thresholdDial()
    ,m_makeupGainDial()
    ,m_rangeDial()
    ,m_outputDial()
    ,m_wetDryDial()
{

    // Set up the soft clipping toggle button
    m_softClippingToggle.setButtonText("Soft Clipping");
    m_softClippingToggle.addListener(this);
    addAndMakeVisible(m_softClippingToggle);

    // Go through each slider and set dial properties
    for (size_t index = 0; index < m_dials.size(); index++)
    {
        setCommonSliderProperties(*m_dials[index]);
        addAndMakeVisible(m_dials[index]);
        m_dials[index]->addListener(this);
    }

    // Set the labels for each dial
    for (size_t index = 0; index < dialLabels.size(); index++)
    {
        dialLabels[index]->attachToComponent(m_dials[index], true);
        dialLabels[index]->setFont(juce::Font(10.0f, juce::Font::plain));
        dialLabels[index]->setJustificationType(juce::Justification::centred);
        addAndMakeVisible(dialLabels[index]);
    }

    // Set the range and skew factors for each dial
    m_inputDial.setRange(0.0, 1.0);
    m_inputDial.setSkewFactorFromMidPoint(0.3);

    m_attackDial.setRange(1.0, 1000.0);
    m_attackDial.setSkewFactorFromMidPoint(100.0);
    m_attackDial.setValue(10.0);

    m_releaseDial.setRange(10.0, 10000.0);
    m_releaseDial.setSkewFactorFromMidPoint(1000.0);
    m_releaseDial.setValue(100.0);

    m_ratioDial.setRange(1.0, 20.0);
    m_ratioDial.setSkewFactorFromMidPoint(2.0);
    m_ratioDial.setValue(4.0);

    m_thresholdDial.setRange(-60.0, 0.0);
    m_thresholdDial.setSkewFactorFromMidPoint(-20.0);
    m_thresholdDial.setValue(-20.0);

    m_makeupGainDial.setRange(0.0, 24.0);
    m_makeupGainDial.setSkewFactorFromMidPoint(6.0);
    m_makeupGainDial.setValue(0.0);

    m_rangeDial.setRange(0.0, 60.0);
    m_rangeDial.setSkewFactorFromMidPoint(30.0);
    m_rangeDial.setValue(30.0);

    m_outputDial.setRange(-24.0, 24.0);
    m_outputDial.setSkewFactorFromMidPoint(0.0);
    m_outputDial.setValue(0.0);

    m_wetDryDial.setRange(0.0, 1.0);
    m_wetDryDial.setSkewFactorFromMidPoint(0.5);
    m_wetDryDial.setValue(0.5);

    setSize(400, 300);

}

MyCompressorEditor::~MyCompressorEditor()
{
    m_dials.clear();
    dialLabels.clear();
    //Shrinks to only the needed elements in the vector
    m_dials.shrink_to_fit();
    dialLabels.shrink_to_fit();
}

void MyCompressorEditor::sliderValueChanged(juce::Slider* slider)
{

}

void MyCompressorEditor::buttonClicked(juce::Button* button)
{
    if (button == &m_softClippingToggle)
    {
    }
}

//==============================================================================
void MyCompressorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setGradientFill(
        juce::ColourGradient::vertical(juce::Colour::fromRGB(202, 222, 220).darker(0.05f)
        ,getHeight()
        ,juce::Colour::fromRGB(202, 222, 220).brighter(0.02f), getHeight() * 0.4));

    ////g.fillEllipse(0.f,0.f,740.f,480.f);
    ////g.fillRect(getLocalBounds());
    ////g.fillRoundedRectangle(0.f,0.f, 750.f,480.f,50.f);
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
// Arrange components within the editor window
const int toggleButtonWidth = 100;
const int toggleButtonHeight = 30;
const int dialWidth = 60;
const int dialHeight = 60;
const int dialLabelHeight = 20;

const int margin = 10;
const int padding = 5;

int x = margin;
int y = margin;

m_softClippingToggle.setBounds(x, y, toggleButtonWidth, toggleButtonHeight);
y += toggleButtonHeight + padding;

int rowHeight = std::max(dialHeight, dialLabelHeight) + padding;

for (size_t index = 0; index < m_dials.size(); index++)
{
    m_dials[index]->setBounds(x, y, dialWidth, dialHeight);
    y += dialHeight + padding;

    dialLabels[index]->setBounds(x, y, dialWidth, dialLabelHeight);
    y += dialLabelHeight + padding;

    if ((index + 1) % 3 == 0)
    {
        x += dialWidth + padding;
        y = margin;
    }
}
}

void MyCompressorEditor::setCommonSliderProperties(juce::Slider& slider)
{
    slider.setLookAndFeel(&m_lnf); // Set the look and feel for the slider
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&m_lnf);
    slider.addListener(this);
}

