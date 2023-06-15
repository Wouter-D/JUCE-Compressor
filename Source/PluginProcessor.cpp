    /*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_dsp/juce_dsp.h>

//==============================================================================
MyGlueCompressor::MyGlueCompressor()
    : AudioProcessor(BusesProperties()


#ifndef JucePlugin_PreferredChannelConfigurations
    
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    m_treeState(*this, nullptr, "Parameters",createParameterLayout()),
    m_attackTime(10.f),
    m_releaseTime(100.f),
    m_ratio(4.f),
    m_threshold(-20.f),
    m_makeupGain(0.0f),
    m_range(20.f),
    m_dryWet(1.f),
    m_softClippingEnabled(false)
#endif
{
    m_treeState.addParameterListener("input", this);
    m_treeState.addParameterListener("thresh", this);
    m_treeState.addParameterListener("ratio", this);
    m_treeState.addParameterListener("attack", this);
    m_treeState.addParameterListener("release", this);
    m_treeState.addParameterListener("output", this);
    m_treeState.addParameterListener("Dry/Wet", this);
}

MyGlueCompressor::~MyGlueCompressor()
{
    m_treeState.removeParameterListener("input", this);
    m_treeState.removeParameterListener("thresh", this);
    m_treeState.removeParameterListener("ratio", this);
    m_treeState.removeParameterListener("attack", this);
    m_treeState.removeParameterListener("release", this);
    m_treeState.removeParameterListener("output", this);
    m_treeState.removeParameterListener("Dry/Wet", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MyGlueCompressor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter >> params;

    juce::NormalisableRange<float> attackRange = juce::NormalisableRange<float>(0.0f, 200.0f, 1.f);
    attackRange.setSkewForCentre(50.f);

    juce::NormalisableRange<float> releaseRange = juce::NormalisableRange<float>(5.0f, 5000.0f, 1.f);
    releaseRange.setSkewForCentre(160.f);

    //juce::NormalisableRange<float> wetDryMixRange = juce::NormalisableRange<float>(5.0f, 5000.0f, 1.f);
    //wetDryMixRange.setSkewForCentre(160.f);

    auto pInput = std::make_unique<juce::AudioParameterFloat>("input", "Input", -60.f, 24.f, 0.f);
    auto pThresh = std::make_unique<juce::AudioParameterFloat>("thresh", "Thresh", -60.f, 10.f, 0.f);
    auto pRatio = std::make_unique<juce::AudioParameterFloat>("ratio", "Ratio", 1.f, 20.f, 1.f);
    auto pAttack = std::make_unique<juce::AudioParameterFloat>("attack", "Attack", attackRange, 50.f);
    auto pRelease = std::make_unique<juce::AudioParameterFloat>("release", "Release", releaseRange, 160.0f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>("output", "Output", -60.f, 24.f, 0.f);
    auto pWetDry = std::make_unique<juce::AudioParameterFloat>("Dry/Wet", "Dry/Wet", 0.f, 10.f, 0.f);

    
    params.push_back(std::move(pInput));
    params.push_back(std::move(pThresh));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pAttack));
    params.push_back(std::move(pRelease));
    params.push_back(std::move(pOutput));
    params.push_back(std::move(pWetDry));

    return { params.begin(), params.end() };
}



void MyGlueCompressor::parameterChanged(const juce::String& parameterID, float newValue)
{
    DBG("input: " << m_treeState.getRawParameterValue("input")->load());
    DBG("tresh: " << m_treeState.getRawParameterValue("thresh")->load());
    DBG("ratio: " << m_treeState.getRawParameterValue("ratio")->load());
    DBG("attack: " << m_treeState.getRawParameterValue("attack")->load());
    DBG("release: " << m_treeState.getRawParameterValue("release")->load());
    DBG("output: " << m_treeState.getRawParameterValue("output")->load());
    DBG("Dry/Wet: " << m_treeState.getRawParameterValue("Dry/Wet")->load());

    updateParameters();

    //Checking why in and output wont immediatly change or update
    m_inputModule.setGainDecibels(m_treeState.getRawParameterValue("input")->load());
    m_outputModule.setGainDecibels(m_treeState.getRawParameterValue("output")->load());
}



void MyGlueCompressor::updateParameters()
{
    //Update all DSP module parameters
    //inputModule.setGainLinear(treeState.getRawParameterValue("input")->load());
    m_inputModule.setGainDecibels(m_treeState.getRawParameterValue("input")->load());
    m_compressorModule.setThreshold(m_treeState.getRawParameterValue("thresh")->load());
    m_compressorModule.setRatio(m_treeState.getRawParameterValue("ratio")->load());
    m_compressorModule.setAttack(m_treeState.getRawParameterValue("attack")->load());
    m_compressorModule.setRelease(m_treeState.getRawParameterValue("release")->load());
    m_outputModule.setGainDecibels(m_treeState.getRawParameterValue("output")->load());

}

float MyGlueCompressor::applyCompression(float input)
{
    // Define the compression parameters
    const float threshold = 0.5f;   // Threshold above which compression is applied
    const float ratio = 2.0f;       // Compression ratio
    const float makeupGain = 2.0f;  // Makeup gain applied to the compressed signal

    // Apply compression
    float output = input;

    if (std::abs(input) > threshold)
    {
        // Apply compression if input exceeds the threshold
        float gainReduction = (std::abs(input) - threshold) / ratio;
        gainReduction = std::min(gainReduction, 1.0f); // Ensure gain reduction is limited to 1.0

        output = input - gainReduction;
    }

    // Apply makeup gain to the compressed signal
    output *= makeupGain;

    return output;
}


//==============================================================================
const juce::String MyGlueCompressor::getName() const
{
    return JucePlugin_Name;
}

bool MyGlueCompressor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyGlueCompressor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyGlueCompressor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyGlueCompressor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyGlueCompressor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyGlueCompressor::getCurrentProgram()
{
    return 0;
}

void MyGlueCompressor::setCurrentProgram (int index)
{
}

const juce::String MyGlueCompressor::getProgramName (int index)
{
    return {};
}

void MyGlueCompressor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyGlueCompressor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need.. 
    m_spec.sampleRate = sampleRate;
    m_spec.maximumBlockSize = samplesPerBlock;
    m_spec.sampleRate = sampleRate;
    m_spec.numChannels = static_cast<uint32_t>(getTotalNumOutputChannels()); //DSP module needs this

    //Prepare DSP
    m_inputModule.prepare(m_spec);
    m_inputModule.setRampDurationSeconds(0.05);//Smooth change from parameter
    m_outputModule.setRampDurationSeconds(0.05);//Smooth change from parameter
    m_compressorModule.prepare(m_spec);
    m_outputModule.prepare(m_spec);
    updateParameters();
}

void MyGlueCompressor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up anyFloat
    // spare memory, etc.
    m_gainProcessor.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyGlueCompressor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
void MyGlueCompressor::setAttackTime(float attackTime)
{
}
void MyGlueCompressor::setReleaseTime(float releaseTime)
{
}
void MyGlueCompressor::setRatio(float ratio)
{
}
void MyGlueCompressor::setThreshold(float Threshold)
{
}
void MyGlueCompressor::setMakeupGain(float makeupGain)
{
}
void MyGlueCompressor::setRange(float range)
{
}
void MyGlueCompressor::setDryWet(float dryWet)
{
}
void MyGlueCompressor::setSoftClippingEnabled(bool enabled)
{
}
#endif

void MyGlueCompressor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    // Process DSP modules
    juce::dsp::AudioBlock<float> block{ buffer };

    m_inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    m_compressorModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    m_outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    //float wetDryMix = m_treeState.getRawParameterValue("Dry/Wet")->load();

    //// Check if the buffer is empty or has no channels
    //if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
    //    return;

    //// Ensure the buffer has at least one channel
    //const int numChannels = buffer.getNumChannels();

    //for (int channel = 0; channel < numChannels; ++channel)
    //{
    //    // Check if the channel number is within the valid range
    //    if (buffer.getArrayOfWritePointers()[channel] == nullptr)
    //        continue;

    //    float* channelData = buffer.getWritePointer(channel);

    //    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    //    {
    //        // Duplicate the input dry channel
    //        float drySignal = channelData[sample];

    //        // Create the wet signal by applying compression or any other effect
    //        float wetSignal = applyCompression(drySignal); // Replace with your compression function or effect processing

    //        juce::dsp::Limiter<float> limit;


    //        // Mix the wet and dry signals
    //        channelData[sample] = m_wetGain * wetSignal * wetDryMix + m_dryGain * drySignal * (1 - wetDryMix);
    //    }
    //}



    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //{
    //    buffer.clear(i, 0, buffer.getNumSamples());
    //}
 
    //// This is the place where you'd normally do the guts of your plugin's
    //// audio processing...
    //// Make sure to reset the state if your inner loop is processing
    //// the samples and the outer loop is handling the channels.
    //// Alternatively, you can process the samples with the channels
    //// interleaved by keeping the same state.

}


//==============================================================================
bool MyGlueCompressor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyGlueCompressor::createEditor()
{
    //return new juce::GenericAudioProcessorEditor(*this);

    return new MyCompressorEditor(*this);
}

//==============================================================================
void MyGlueCompressor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    //Save params to memory stream
    juce::MemoryOutputStream stream(destData, false);
    m_treeState.state.writeToStream(stream);
}

void MyGlueCompressor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //Recall params from memory stream
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        m_treeState.state = tree;

    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyGlueCompressor();
}
