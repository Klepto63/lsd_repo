//vst proc.h
/*

#pragma once
#include <JuceHeader.h>

using namespace juce;
//==============================================================================

class MyAudioProcessorGraph : public AudioProcessor
{

	using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
	using Node = AudioProcessorGraph::Node;	

public :

	MyAudioProcessorGraph():
		AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true)
										.withOutput("Output",juce::AudioChannelSet::stereo(),true)),
		mainProcessor(new juce::AudioProcessorGraph())
	{

	};


	~MyAudioProcessorGraph()
	{

	};

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override
	{
		//only mono to mono OR stereo to stereo vst are ok 
		if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
			|| layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
			return false;
		if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
			&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
			return false;
		return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
	}

	void prepareToPlay(double sampleRate, int samplesPerBlock) override
	{
		//specify number of input, output, samplerate, blockSize
		mainProcessor->setPlayConfigDetails(	getMainBusNumInputChannels(),
												getMainBusNumOutputChannels(),
												sampleRate, samplesPerBlock);

		mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);


		//initialise audiograph / nodes
		mainProcessor->clear();
		audioInputNODE = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
		audioOutputNODE = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
		midiInputNODE = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
		midiOutputNODE = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));

		//connect audio node (channel 0 et 1)
		mainProcessor->addConnection({ {audioInputNODE->nodeID, 0}  , {audioOutputNODE->nodeID, 0}  });
	
		//connectMidiNodes();
		//todo?
	}

	void releaseResources() override
	{
		mainProcessor->releaseResources();
	}



	void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override
	{
		//clear unused channel
		for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		{
			buffer.clear(i, 0, buffer.getNumSamples());
		}

		updateGraph();

		mainProcessor->processBlock(buffer, midiMessages);
	}


private :

	void updateGraph(void)
	{

	}

	Node::Ptr audioInputNODE;
	Node::Ptr audioOutputNODE;

	//
	Node::Ptr midiInputNODE;
	Node::Ptr midiOutputNODE;

	std::unique_ptr <juce::AudioProcessorGraph> mainProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAudioProcessorGraph);
};
*/
