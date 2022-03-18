#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "JuceHeader.h"
#include "codaFront.h"
#include "pathsong.h"
#include "PlayerTitlePlayingComponent.h"
#include "EnergySliderLookAndFeel.h"
#include "JsonParser.h"
#include "Path.h"

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

private:
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
    PlayerComponent() : mainProcessor (new juce::AudioProcessorGraph()), state(Stopped)
    {
        currentIdxPlaying = 0;
        currentVolume = 85;
        isMuted = false;
        musicSliderBlockTimer = false; 

        addAndMakeVisible(&energySlider);
        energySlider.setSliderStyle(Slider::Rotary);
        energySlider.setValue(5, juce::dontSendNotification); //1..10
        energySlider.setLookAndFeel(&energySliderLookAndFeel);
        energySlider.hideTextBox(true);
        energySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        energySlider.addListener(this);
        energySlider.setRotaryParameters(MathConstants<float>::pi * 1.5f, MathConstants<float>::pi * 2.5f, true);
        energySlider.setEnabled(false);

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

        addAndMakeVisible(&nextButton);
        nextButton.setButtonText("Next");
        Image imgNextButton = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "nextbutton.png"));
        Image imgNextButton2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"nextButton2.png"));
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
        Image imgPrevButton = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "prevbutton.png"));
        Image imgPrevButton2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String)"prevbutton2.png"));
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

        addAndMakeVisible (CBScenes);
        CBScenes.setColour(ComboBox::backgroundColourId,Colour((uint32)BUTTON_COLOR1));
        CBScenes.setColour(ComboBox::outlineColourId,Colour((uint32)BUTTON_COLOR1));        
                                    //backgroundColourId     = 0x1000b00,   /**< The background colour to fill the box with. */
                                    //textColourId           = 0x1000a00,   /**< The colour for the text in the box. */
                                    //outlineColourId        = 0x1000c00,   /**< The colour for an outline around the box. */
                                    //buttonColourId         = 0x1000d00,   /**< The base colour for the button (a LookAndFeel class will probably use variations on this). */
                                    //arrowColourId          = 0x1000e00,   /**< The colour for the arrow shape that pops up the menu */
                                    //focusedOutlineColourId = 0x1000f00    /**< The colour that will be used to draw a box around the edge of the component when it has focus. */

        CBScenes.addItem("Opera 1", 1);
        CBScenes.addItem("Opera 2", 2);
        CBScenes.addItem("Opera 3", 3);        
        CBScenes.onChange = [this] { CBScenesChanged(); };
        CBScenes.setSelectedId(1);
        CBScenes.setVisible(false);

        addAndMakeVisible(&volumeSlider);
        volumeSlider.hideTextBox(true);
        volumeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        volumeSlider.setValue(currentVolume/10, juce::dontSendNotification);
        volumeSlider.setColour(0x1001200, Colour(BACKGROUND_COLOR)); //backgroundId
        volumeSlider.setColour(0x1001300, Colour(THUMB_COLOR));      //thumbColor (la boule)
        volumeSlider.setColour(0x1001310, Colour(LIGNE_COLOR));      //ligneColor
        volumeSlider.addListener(this);
        addAndMakeVisible(&playerTitlePlayingComponent);
        setSize(200, 200);


    	// Adding the two primary audio sources to the mixer
	    mixerAudioSource.addInputSource(&audio1, false);
	    mixerAudioSource.addInputSource(&audio2, false);
	    mixerAudioSource.addInputSource(&audio3, false);
	    mixerAudioSource.addInputSource(&audio4, false);
        formatManager.registerBasicFormats();
        //transportSource.addChangeListener(this);
        //mixerAudioSource.addChangeListener(this);


		//vstformatManager->addDefaultFormats();


        //AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Couldn't create plugin", "error");

        //auto vstHost = new PluginListComponent((juce::AudioPluginFormatManager&) vstformatManager, (juce::KnownPluginList&) list_, NULL, NULL, true);

		//new PluginListComponent (manager_->formatManager, *manager_->list_, deadMansPedalFile, manager_->props_->getUserSettings(), true), true);

		//		ScopedPointer<XmlElement> pluginList(userSettings->getXmlValue("pluginList"));
 		//if (nullptr != pluginList)
 		//{
    	//	gKnownPlugins.recreateFromXml(*pluginList);
 		//}


        
		addAndMakeVisible(&DebugButton);
		DebugButton.onClick = [this] { DebugButtonCallback(); };
		DebugButton.setEnabled(true);
		DebugButton.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		DebugButton.setButtonText("Debug");
		DebugButton.setVisible(true);


        auto inputDevice  = juce::MidiInput::getDefaultDevice();
        auto outputDevice = juce::MidiOutput::getDefaultDevice();
        mainProcessor->enableAllBuses();

        deviceManager.initialiseWithDefaultDevices (0, 2);                          // [1]
        deviceManager.addAudioCallback (&mainProcessorPlayer);                                   // [2]
        deviceManager.setMidiInputDeviceEnabled (inputDevice.identifier, true);
        deviceManager.addMidiInputDeviceCallback (inputDevice.identifier, &mainProcessorPlayer); // [3]
        deviceManager.setDefaultMidiOutputDevice (outputDevice.identifier);

        initialiseGraph();

        mainProcessorPlayer.setProcessor (mainProcessor.get()); //definie quel AudioProcessorGraph le player doit sortir



        setAudioChannels(2, 2);
        startTimer(20);
    }
    ~PlayerComponent() override
    {
        shutdownAudio();
    }



    void sliderDragStarted(Slider* slider);
    void sliderDragEnded(Slider* slider);
    void sliderValueChanged(Slider* slider);
    void playCommand(void);
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void paint(juce::Graphics& g) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void updateLoopState(bool shouldLoop);
    bool isPlaying(void);
    void loadAndPlay(int idx);
    void initialiseGraph();
    void addPluginCallback(std::unique_ptr<AudioPluginInstance> instance, const String& error);
private:

    int     currentIdxPlaying;
    int     currentVolume;
    int     volumeBeforeMute;
    bool    isMuted;
    
    bool    musicSliderBlockTimer; //gestion drag drop timer

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };



    void DebugButtonCallback(void)
    {
        AudioProcessorGraph::Node* node;
        node = mainProcessor->getNodeForId((juce::AudioProcessorGraph::NodeID) 4);
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

    void changeState(TransportState newState);
    void updatePlayerButtonImage(bool playing);
    void updateVolumeButtonImage(bool isMuted, int sliderVolume);
    void muteButtonClicked(void);
    void playButtonClicked(void);
    void nextButtonClicked(void);
    void prevButtonClicked(void);
    void CBScenesChanged(void);


    //==========================================================================

    EnergySliderLookAndFeel energySliderLookAndFeel;
    juce::ImageButton playButton;
    juce::ImageButton nextButton;
    juce::ImageButton prevButton;
    juce::ImageButton muteButton;
    juce::Slider musicSlider;
    juce::Slider energySlider;
    juce::Slider volumeSlider;
    juce::Label currentPositionLabel;
    juce::Label lengthLabel;
    PlayerTitlePlayingComponent playerTitlePlayingComponent;
    TransportState state;
    juce::ComboBox   CBScenes;

    juce::TextButton DebugButton;


    juce::AudioFormatManager formatManager;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_audio1;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_audio2;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_audio3;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource_audio4;
    juce::AudioTransportSource  audio1;
    juce::AudioTransportSource  audio2;
    juce::AudioTransportSource  audio3;
    juce::AudioTransportSource  audio4;
    juce::MixerAudioSource mixerAudioSource;


	juce::KnownPluginList* list_;
	juce::AudioPluginFormatManager vstformatManager;


	std::unique_ptr <juce::AudioProcessorGraph> mainProcessor;
    juce::AudioDeviceManager deviceManager;
    juce::AudioProcessorPlayer mainProcessorPlayer; //An AudioIODeviceCallback object which streams audio through an AudioProcessor.
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