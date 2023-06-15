/*
  ==============================================================================

    GroupingProperties.cpp
    Created: 15 Jun 2023 8:13:21pm
    Author:  woute

  ==============================================================================
*/

#include "PluginEditor.h"

void MyCompressorEditor::setGroupProps(juce::GroupComponent& group)
{
    addAndMakeVisible(group);
    ioGroup.setText("IO");
    compressorGroup.setText("Compressor");
    limiterGroup.setText("Limiter");
}
