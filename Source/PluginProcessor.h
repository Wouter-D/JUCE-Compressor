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
class EnvelopeFollower
{
public:
    void reset()
    {
        envelope_ = 0.0f;
    }

    void process(const float* buffer, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float absoluteSample = std::abs(buffer[i]);
            if (absoluteSample > envelope_)
                envelope_ = absoluteSample;
            else
                envelope_ *= 0.99f; // Decay the envelope over time
        }
    }

    float getEnvelope() const
    {
        return envelope_;
    }

private:
    float envelope_ = 0.0f;
};

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

    void setAttackTime(float attackTime);
    void setReleaseTime(float releaseTime);
    void setRatio(float ratio);
    void setThreshold(float Threshold);
    void setMakeupGain(float makeupGain);
    void setRange(float range);
    void setDryWet(float dryWet);
    void setSoftClippingEnabled(bool enabled);

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

    //Parameters
    float m_attackTime{0.0f};
    float m_releaseTime{ 0.0f };
    float m_ratio{ 0.0f };
    float m_Threshold{ 0.0f };
    float m_makeupGain{ 0.0f };
    float m_range{ 0.0f };
    float m_dryWet{ 0.0f };
    bool m_softClippingEnabled{false};

    EnvelopeFollower m_envelopeFollower;

    float m_wetLevel;  // Wet level in decibels
    float m_dryLevel;  // Dry level in decibels

    float m_wetGain = 0.5f;
    float m_dryGain = 0.5f;

    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyGlueCompressor)



};


// check https://www.youtube.com/watch?v=NOYk7O9EJjo for tutorial-