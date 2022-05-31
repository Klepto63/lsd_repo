#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "JuceHeader.h"
#include "codaFront.h"
#include "pathsong.h"
#include "PlayerTitlePlayingComponent.h"

#include "JsonParser.h"
#include "Path.h"
#include "sceneconfig.h"
#include "vstProcessor.h"

using namespace juce;



//==============================================================================
/**
    A desktop window containing a plugin's GUI.
*/
class PluginWindow  : public DocumentWindow
{
public:
    enum class Type
    {
        normal = 0,
        generic,
        programs,
        audioIO,
        debug,
        numTypes
    };

    PluginWindow (AudioProcessorGraph::Node* n, Type t, OwnedArray<PluginWindow>& windowList)
       : DocumentWindow (n->getProcessor()->getName(),
                         LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                         DocumentWindow::minimiseButton | DocumentWindow::closeButton),
         activeWindowList (windowList),
         node (n), type (t)
    {
        setSize (400, 300);

        if (auto* ui = createProcessorEditor (*node->getProcessor(), type))
        {
            setContentOwned (ui, true);
            setResizable (ui->isResizable(), false);
        }

       #if JUCE_IOS || JUCE_ANDROID
        auto screenBounds = Desktop::getInstance().getDisplays().getTotalBounds (true).toFloat();
        auto scaleFactor = jmin ((screenBounds.getWidth() - 50) / getWidth(), (screenBounds.getHeight() - 50) / getHeight());

        if (scaleFactor < 1.0f)
            setSize ((int) (getWidth() * scaleFactor), (int) (getHeight() * scaleFactor));

        setTopLeftPosition (20, 20);
       #else
        setTopLeftPosition (node->properties.getWithDefault (getLastXProp (type), Random::getSystemRandom().nextInt (500)),
                            node->properties.getWithDefault (getLastYProp (type), Random::getSystemRandom().nextInt (500)));
       #endif

        node->properties.set (getOpenProp (type), true);

        setVisible (true);
    }

    ~PluginWindow() override
    {
        clearContentComponent();
    }

    void moved() override
    {
        node->properties.set (getLastXProp (type), getX());
        node->properties.set (getLastYProp (type), getY());
    }

    void closeButtonPressed() override
    {
        node->properties.set (getOpenProp (type), false);
        activeWindowList.removeObject (this);
    }

    static String getLastXProp (Type type)    { return "uiLastX_" + getTypeName (type); }
    static String getLastYProp (Type type)    { return "uiLastY_" + getTypeName (type); }
    static String getOpenProp  (Type type)    { return "uiopen_"  + getTypeName (type); }
    OwnedArray<PluginWindow>& activeWindowList;
    const AudioProcessorGraph::Node::Ptr node;
    const Type type;

    BorderSize<int> getBorderThickness() override
    {
       #if JUCE_IOS || JUCE_ANDROID
        const int border = 10;
        return { border, border, border, border };
       #else
        return DocumentWindow::getBorderThickness();
       #endif
    }

private:
    float getDesktopScaleFactor() const override     { return 1.0f; }

    static AudioProcessorEditor* createProcessorEditor (AudioProcessor& processor,
                                                        PluginWindow::Type type)
    {
        if (type == PluginWindow::Type::normal)
        {
            if (processor.hasEditor())
                if (auto* ui = processor.createEditorIfNeeded())
                    return ui;

            type = PluginWindow::Type::generic;
        }

        if (type == PluginWindow::Type::generic)  return new GenericAudioProcessorEditor (processor);
        if (type == PluginWindow::Type::programs) return new ProgramAudioProcessorEditor (processor);
        //if (type == PluginWindow::Type::audioIO)  return new IOConfigurationWindow (processor);
        //if (type == PluginWindow::Type::debug)    return new PluginDebugWindow (processor);

        jassertfalse;
        return {};
    }

    static String getTypeName (Type type)
    {
        switch (type)
        {
            case Type::normal:     return "Normal";
            case Type::generic:    return "Generic";
            case Type::programs:   return "Programs";
            case Type::audioIO:    return "IO";
            case Type::debug:      return "Debug";
            case Type::numTypes:
            default:               return {};
        }
    }

    //==============================================================================
    struct ProgramAudioProcessorEditor  : public AudioProcessorEditor
    {
        ProgramAudioProcessorEditor (AudioProcessor& p)  : AudioProcessorEditor (p)
        {
            setOpaque (true);
            addAndMakeVisible (panel);
            Array<PropertyComponent*> programs;
            auto numPrograms = p.getNumPrograms();
            int totalHeight = 0;
            for (int i = 0; i < numPrograms; ++i)
            {
                auto name = p.getProgramName (i).trim();
                if (name.isEmpty())
                    name = "Unnamed";
                auto pc = new PropertyComp (name, p);
                programs.add (pc);
                totalHeight += pc->getPreferredHeight();
            }
            panel.addProperties (programs);
            setSize (400, jlimit (25, 400, totalHeight));
        }
        void paint (Graphics& g) override
        {
            g.fillAll (Colours::grey);
        }
        void resized() override
        {
            panel.setBounds (getLocalBounds());
        }
    private:
        struct PropertyComp  : public PropertyComponent,
                               private AudioProcessorListener
        {
            PropertyComp (const String& name, AudioProcessor& p)  : PropertyComponent (name), owner (p)
            {
                owner.addListener (this);
            }
            ~PropertyComp() override
            {
                owner.removeListener (this);
            }
            void refresh() override {}
            void audioProcessorChanged (AudioProcessor*, const ChangeDetails&) override {}
            void audioProcessorParameterChanged (AudioProcessor*, int, float) override {}
            AudioProcessor& owner;
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyComp)
        };
        PropertyPanel panel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgramAudioProcessorEditor)
    };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};



//==============================================================================
class ProcessorBase  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ProcessorBase()
        : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo())
                                           .withOutput ("Output", juce::AudioChannelSet::stereo()))
    {}

    //==============================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override {}

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override          { return nullptr; }
    bool hasEditor() const override                              { return false; }

    //==============================================================================
    const juce::String getName() const override                  { return {}; }
    bool acceptsMidi() const override                            { return false; }
    bool producesMidi() const override                           { return false; }
    double getTailLengthSeconds() const override                 { return 0; }

    //==============================================================================
    int getNumPrograms() override                                { return 0; }
    int getCurrentProgram() override                             { return 0; }
    void setCurrentProgram (int) override                        {}
    const juce::String getProgramName (int) override             { return {}; }
    void changeProgramName (int, const juce::String&) override   {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override       {}
    void setStateInformation (const void*, int) override         {}

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};



//==============================================================================
class OscillatorProcessor  : public ProcessorBase
{
public:
    OscillatorProcessor()
    {
        oscillator.setFrequency (480.0f);
        oscillator.initialise ([] (float x) { return (0.028f* std::sin (x)); });
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
        oscillator.prepare (spec);
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        oscillator.process (context);
    }

    void reset() override
    {
        oscillator.reset();
    }

    const juce::String getName() const override { return "Oscillator"; }


    void setMyFrequency(float x)
    {
        oscillator.setFrequency (x);
    }

private:
    juce::dsp::Oscillator<float> oscillator;
};



//==============================================================================
class CustomPlayerProcessor  : public ProcessorBase, public  juce::ChangeListener
{
public:

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    CustomPlayerProcessor(int idx)
    {
        id = idx;
        state = Stopped;
        formatManager.registerBasicFormats();
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        audioX.prepareToPlay(samplesPerBlock,sampleRate);
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
   
        auto totalNumInputChannels = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        {
            buffer.clear(i, 0, buffer.getNumSamples());
        }

       audioX.getNextAudioBlock(AudioSourceChannelInfo(buffer));
       for (int channel = 0; channel < totalNumInputChannels; ++channel)
       {
        //DSP Filtering code comes here
       }
    }


/*
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        if (readerSource_audioX.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }


    }
*/

    void reset() override
    {
    }

    void changeState(TransportState newState)
    {
        int lengthDuration_s;
        int minutes; 
        int seconds;
        String LengthString;
        if (state != newState)
        {
            state = newState;
            switch (state)
            {
            case Stopped:
                audioX.setPosition(0.0);
                break;
            case Starting:
                audioX.start();
                break;
            case Playing:
                break;
            case Stopping:
                audioX.stop();          
                break;
            }
        }
    }

    bool isPlaying(void)
    {
        return audioX.isPlaying();
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {

    }

    void setPosition(float v)
    {
        audioX.setPosition(v * audioX.getLengthInSeconds() / 10);
    }

    void setGain(float v)
    {
        audioX.setGain(v);
    }

    void loadAndPlay(int idx, int stemidx) //stem 0
    {
        s_metadata md;
        if (!jsonParserLoad(idx, &md))
        {
            auto* reader_audio1 = formatManager.createReaderFor(File(md.stem[stemidx].path));
            if (reader_audio1 != nullptr)
            {
                std::unique_ptr<juce::AudioFormatReaderSource> newSource1(new juce::AudioFormatReaderSource(reader_audio1, true));
                audioX.setSource(newSource1.get(), 0, nullptr, reader_audio1->sampleRate);
                readerSource_audioX.reset(newSource1.release());
                changeState(Starting);
            }
        }
        else
        {
            //song not found
        }
    }


    double getLength_s()
    {
        return audioX.getLengthInSeconds();
    }

    double getCurrentPosition_s()
    {
        return audioX.getCurrentPosition();
    }

    const juce::String getName() const override { return "Player"; }
    const int getId() const {return id;} 

private:
    TransportState state;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_audioX;
    juce::AudioTransportSource audioX;
    juce::AudioFormatManager formatManager;
    int currentVolume = 85;

    int id = 0;
    juce::dsp::Oscillator<float> oscillator;
};




















































//==============================================================================
class PlayerComponent : public juce::AudioAppComponent,
    public  juce::ChangeListener,
    public  juce::Timer,
    private juce::Slider::Listener
{

	using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
	using Node = AudioProcessorGraph::Node;	




public:
    enum Master_TransportState
    {
        Master_Stopped,
        Master_Starting,
        Master_Playing,
        Master_Stopping
    };

    PlayerComponent() : mainProcessor (new juce::AudioProcessorGraph())
    {
        currentIdxPlaying = 0;
        isMuted = false;
        musicSliderBlockTimer = false; 


        addAndMakeVisible(&playButton);
        playButton.setButtonText("Play");
        updatePlayerButtonImage(false);
        playButton.onClick = [this] { playButtonClicked(); };
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        playButton.setEnabled(false);

        addAndMakeVisible(&muteButton);
        muteButton.setButtonText("Mute");
        updateVolumeButtonImage(false, 100);
        muteButton.onClick = [this] { muteButtonClicked(); };
        muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);

        addAndMakeVisible(&cubeButton);
        cubeButton.setButtonText("Scene");
        cubeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);   
        Image imgcubeButton = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_3D_BUTTON)));
        Image imgcubeButton2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_3D_BUTTON_ONCLICK)));
        cubeButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
            imgcubeButton, 1.0f, juce::Colours::transparentBlack,
            imgcubeButton, 1.0f, juce::Colours::white,
            imgcubeButton2, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );

        addAndMakeVisible(&nextButton);
        nextButton.setButtonText("Next");
        Image imgNextButton = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_NEXT_BUTTON)));
        Image imgNextButton2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_NEXT_BUTTON_ONCLICK)));
        nextButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
            imgNextButton, 1.0f, juce::Colours::transparentBlack,
            imgNextButton, 1.0f, juce::Colours::white,
            imgNextButton2, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );
        nextButton.onClick = [this] { nextButtonClicked(); };
        nextButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);

        addAndMakeVisible(&prevButton);
        prevButton.setButtonText("Prev");
        Image imgPrevButton = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_PREV_BUTTON)));
        Image imgPrevButton2 = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_IMG_PREV_BUTTON_ONCLICK)));
        prevButton.setImages(true,  //resize to fit
            true,  //rescale image
            true,  //preserve proportion
            imgPrevButton, 1.0f, juce::Colours::transparentBlack,
            imgPrevButton, 1.0f, juce::Colours::white,
            imgPrevButton2, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );
        prevButton.onClick = [this] { prevButtonClicked(); };
        prevButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        addAndMakeVisible(&currentPositionLabel);
        currentPositionLabel.setFont(12);
        addAndMakeVisible(&lengthLabel);
        lengthLabel.setJustificationType(Justification::centredLeft);
        lengthLabel.setFont(12);
        addAndMakeVisible(&musicSlider);
        musicSlider.hideTextBox(true);
        //musicSlider.setSliderStyle(Slider::LinearBar);
        musicSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        //musicSlider.setMaxValue(10, dontSendNotification);
        musicSlider.setColour(0x1001200, Colour(BACKGROUND_COLOR)); //backgroundId
        musicSlider.setColour(0x1001300, Colour(THUMB_COLOR));      //thumbColor (la boule)
        musicSlider.setColour(0x1001310, Colour(LIGNE_COLOR));      //ligneColor
        musicSlider.addListener(this);

        addAndMakeVisible(&volumeSlider);
        volumeSlider.hideTextBox(true);
        volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        //volumeSlider.setValue(currentVolume/10, juce::dontSendNotification);
        volumeSlider.setValue(currentVolume / 10, juce::dontSendNotification);
        volumeSlider.setColour(0x1001200, Colour(BACKGROUND_COLOR)); //backgroundId
        volumeSlider.setColour(0x1001300, Colour(THUMB_COLOR));      //thumbColor (la boule)
        volumeSlider.setColour(0x1001310, Colour(LIGNE_COLOR));      //ligneColor
        volumeSlider.addListener(this);
        addAndMakeVisible(&playerTitlePlayingComponent);
        setSize(200, 200);

		//addAndMakeVisible(&DebugButton);
		DebugButton.onClick = [this] { DebugButtonCallback(); };
		DebugButton.setEnabled(true);
		DebugButton.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		DebugButton.setButtonText("Debug");
		DebugButton.setVisible(true);
//
		//addAndMakeVisible(&DebugButton2);
		DebugButton2.onClick = [this] { DebugButton2Callback(); };
		DebugButton2.setEnabled(true);
		DebugButton2.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		DebugButton2.setButtonText("Debug2");
		DebugButton2.setVisible(true);

        auto inputDevice  = juce::MidiInput::getDefaultDevice();
        auto outputDevice = juce::MidiOutput::getDefaultDevice();
        mainProcessor->enableAllBuses();
        deviceManager.initialiseWithDefaultDevices (0, 2);                          // [1]
        deviceManager.addAudioCallback (&mainProcessorPlayer);                                   // [2]
        deviceManager.setMidiInputDeviceEnabled (inputDevice.identifier, true);
        deviceManager.addMidiInputDeviceCallback (inputDevice.identifier, &mainProcessorPlayer); // [3]
        deviceManager.setDefaultMidiOutputDevice (outputDevice.identifier);

        initialiseGraph();

        Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);

        //====delai
        

        mainProcessorPlayer.setProcessor (mainProcessor.get()); //definie quel AudioProcessorGraph le player doit sortir
        setAudioChannels(0, 2);
        startTimer(20);
    }
    ~PlayerComponent() override
    {
        shutdownAudio();
    }

    void setAngle(float f);
    void sliderDragStarted(Slider* slider);
    void sliderDragEnded(Slider* slider);
    void sliderValueChanged(Slider* slider);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;


    void Master_loadAndPlay(int idx);
    //void PlayerComponent::Master_stopPlay(void);
    void Master_changeState(Master_TransportState Master_newState);
    bool Master_isPlaying(void);

    void applyHightlight(void);
    void updateConfig(void);

    void initialiseGraph();
    void addPluginCallback(std::unique_ptr<AudioPluginInstance> instance, const String& error, int ii);



private:

    void DebugButtonCallback(void)
    {
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 8);
        if(node != nullptr)
        {
            if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
                    {
                        auto description = plugin->getPluginDescription();
                       //if (! plugin->hasEditor() && description.pluginFormatName == "Internal")
                       //{
                       //    getCommandManager().invokeDirectly (CommandIDs::showAudioSettings, false);
                       //    return nullptr;
                       //}
                        //auto localDpiDisabler = makeDPIAwarenessDisablerForPlugin (description);
                        activePluginWindows.add (new PluginWindow (node, PluginWindow::Type::normal, activePluginWindows));
                        //activePluginWindows.add(new PluginWindow(node, PluginWindow::Type::generic, activePluginWindows));
                    }
                }
        }
        //show dear
        //auto NodeDearVr = mainProcessor->getNode(4);
        //auto editor = NodeDearVr->createEditor();
        //auto bc = editor->getConstrainer();
        //editor->setBounds(0, 0, bc->getMinimumWidth(), bc->getMinimumHeight());
        //addAndMakeVisible (editor);
    }

    void DebugButton2Callback(void)
    {
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 8);
        if(node != nullptr)
        {
            if (auto* processor = node->getProcessor())
                {
                    if (auto* plugin = dynamic_cast<AudioPluginInstance*> (processor))
                    {
                        auto description = plugin->getPluginDescription();
                        auto parameters = plugin->getParameters();
                        auto test = plugin->getNumParameters();
                        int ii = 0;
                        for (int i= 0; i < test; i++)
                        {
                            auto test2 = plugin->getHostedParameter(i);
                            auto test3 = test2->getName(20);
                            ii++;
                        }
                        activePluginWindows.add (new PluginWindow (node, PluginWindow::Type::generic, activePluginWindows));
                    }
                }
        }
    } 

    void updatePlayerButtonImage(bool playing);
    void updateVolumeButtonImage(bool isMuted, int sliderVolume);
    void muteButtonClicked(void);
    void playButtonClicked(void);
    void nextButtonClicked(void);
    void prevButtonClicked(void);
    void cubeButtonClicked(void);

    //==========================================================================
    Master_TransportState   Master_state;
    int                     currentIdxPlaying;
    int                     volumeBeforeMute;
    bool                    isMuted;
    bool                    musicSliderBlockTimer; //gestion drag drop timer
    int                     currentVolume = 85;                   


    juce::ImageButton playButton;
    juce::ImageButton nextButton;
    juce::ImageButton prevButton;
    juce::ImageButton muteButton;
    juce::ImageButton cubeButton;
    juce::Slider musicSlider;
    juce::Slider volumeSlider;
    juce::Label currentPositionLabel;
    juce::Label lengthLabel;
    PlayerTitlePlayingComponent playerTitlePlayingComponent;

    juce::TextButton DebugButton;
    juce::TextButton DebugButton2;

	juce::KnownPluginList* list_;

	juce::AudioPluginFormatManager vstformatManager1;
	juce::AudioPluginFormatManager vstformatManager2;
	juce::AudioPluginFormatManager vstformatManager3;
	juce::AudioPluginFormatManager vstformatManager4;
	juce::AudioPluginFormatManager vstformatManager5;


	std::unique_ptr <juce::AudioProcessorGraph> mainProcessor;
    juce::AudioDeviceManager deviceManager;
    juce::AudioProcessorPlayer mainProcessorPlayer; 
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;

    OwnedArray<PluginWindow> activePluginWindows;

    juce::ReferenceCountedArray<Node> slots;

    juce::ReferenceCountedArray<Node> activeSlots;    
    Node::Ptr slot1Node;    //osci
    Node::Ptr slot2Node;    //dearVr

 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerComponent)
};


#endif