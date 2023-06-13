/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MyGlueCompressor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    MyGlueCompressor();
    ~MyGlueCompressor() override;
  
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Added stuff

    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS m_treeState;
    static APVTS::ParameterLayout createParameterLayout();
    //APVTS apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:
    void updateParameters();
    float applyCompression(float input);
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::dsp::Gain<float> m_inputModule;
    juce::dsp::Gain<float> m_outputModule;
    juce::dsp::Gain<float> m_gainProcessor;
    juce::dsp::ProcessSpec m_spec;
    juce::dsp::Compressor <float> m_compressorModule; 
    juce::AudioBuffer<float> m_outputBuffer;  // Output buffer
    juce::SmoothedValue<float> m_inputSoftClipping;
    float m_wetLevel;  // Wet level in decibels
    float m_dryLevel;  // Dry level in decibels

    float m_wetGain = 0.5f;
    float m_dryGain = 0.5f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyGlueCompressor)



};


// check https://www.youtube.com/watch?v=NOYk7O9EJjo for tutorial-