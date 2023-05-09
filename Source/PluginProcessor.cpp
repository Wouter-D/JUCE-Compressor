    /*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MyGlueCompressor::MyGlueCompressor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
    , treeState(*this, nullptr, "Parameters",createParameterLayout())
#endif
{
    treeState.addParameterListener("thresh", this);
    treeState.addParameterListener("ratio", this);
    treeState.addParameterListener("attack", this);
    treeState.addParameterListener("release", this);
}

MyGlueCompressor::~MyGlueCompressor()
{
    treeState.removeParameterListener("thresh", this);
    treeState.removeParameterListener("ratio", this);
    treeState.removeParameterListener("attack", this);
    treeState.removeParameterListener("release", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MyGlueCompressor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter >> params;

    juce::NormalisableRange<float> attackRange = juce::NormalisableRange<float>(0.0f, 200.0f, 1.f);
    attackRange.setSkewForCentre(50.f);

    juce::NormalisableRange<float> releaseRange = juce::NormalisableRange<float>(5.0f, 5000.0f, 1.f);
    releaseRange.setSkewForCentre(160.f);

    auto pInput = std::make_unique<juce::AudioParameterFloat>("input", "Input", -60.f, 24.f, 0.f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
    auto pThresh = std::make_unique<juce::AudioParameterFloat>("thresh", "Thresh", -60.f, 10.f, 0.f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
    auto pRatio = std::make_unique<juce::AudioParameterFloat>("ratio", "Ratio", 1.f, 20.f, 1.f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
    auto pAttack = std::make_unique<juce::AudioParameterFloat>("attack", "Attack", attackRange, 50.f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
    auto pRelease = std::make_unique<juce::AudioParameterFloat>("release", "Release", releaseRange, 160.0f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
    auto pOutput = std::make_unique<juce::AudioParameterFloat>("output", "Output", -60.f, 24.f, 0.f);//CHANGE THIS Pointer member viar want b;lijft dit callen en memory leak
   
    
    params.push_back(std::move(pInput));
    params.push_back(std::move(pThresh));
    params.push_back(std::move(pRatio));
    params.push_back(std::move(pAttack));
    params.push_back(std::move(pRelease));
    params.push_back(std::move(pOutput));

    return { params.begin(), params.end() };

}



void MyGlueCompressor::parameterChanged(const juce::String& parameterID, float newValue)
{
    //DBG("input: " << treeState.getRawParameterValue("input")->load());
    //DBG("tresh: " << treeState.getRawParameterValue("thresh")->load());
    //DBG("ratio: " << treeState.getRawParameterValue("ratio")->load());
    //DBG("attack: " << treeState.getRawParameterValue("attack")->load());
    //DBG("release: " << treeState.getRawParameterValue("release")->load());
    //DBG("output: " << treeState.getRawParameterValue("output")->load());

    updateParameters();
}

void MyGlueCompressor::updateParameters()
{

    //Update all DSP module parameters
    inputModule.setGainDecibels(treeState.getRawParameterValue("input")->load());

    compressorModule.setThreshold(treeState.getRawParameterValue("thresh")->load());
    compressorModule.setRatio(treeState.getRawParameterValue("ratio")->load());
    compressorModule.setAttack(treeState.getRawParameterValue("attack")->load());
    compressorModule.setRelease(treeState.getRawParameterValue("release")->load());

    outputModule.setGainDecibels(treeState.getRawParameterValue("output")->load());

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

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels(); //DSP module needs this

    inputModule.prepare(spec);
    inputModule.setRampDurationSeconds(0.02);//Smooth change from parameter
    outputModule.setRampDurationSeconds(0.02);//Smooth change from parameter
    outputModule.prepare(spec);

    compressorModule.prepare(spec);
    updateParameters();
}

void MyGlueCompressor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up anyFloat
    // spare memory, etc.
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

    juce::dsp::AudioBlock<float> block{buffer};
    inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    compressorModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }

    // Added

}


//==============================================================================
bool MyGlueCompressor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyGlueCompressor::createEditor()
{
    //return new MyParallelCompressor (*this);

    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MyGlueCompressor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    ////Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream(stream);
}

void MyGlueCompressor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    ////Recall params
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
       treeState.state = tree;

    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyGlueCompressor();
}
