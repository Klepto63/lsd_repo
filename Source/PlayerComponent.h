#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "JuceHeader.h"
#include "codaFront.h"
#include "pathsong.h"
#include "PlayerTitlePlayingComponent.h"
#include "EnergySliderLookAndFeel.h"
#include "JsonParser.h"
#include "Path.h"

using namespace juce;

//==============================================================================
class PlayerComponent : public juce::AudioAppComponent,
    public  juce::ChangeListener,
    public  juce::Timer,
    private juce::Slider::Listener
{
public:
    PlayerComponent()
        : state(Stopped)
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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerComponent)
};


#endif