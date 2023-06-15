/*
  ==============================================================================

    SliderProps.cpp
    Created: 15 Jun 2023 3:46:36pm
    Author:  woute

  ==============================================================================
*/

#include "../../PluginEditor.h"

void MyCompressorEditor::setCommonSliderProperties(juce::Slider& slider)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 72, 36);

}
