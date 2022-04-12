#pragma once
using namespace juce;

#include "table.h"
#include "LeftBarComponent.h"
#include "playerComponent.h"
#include "SceneComponent.h" 
#include "JsonParser.h"
#include "wnd_sceneconfig.h"
//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    public juce::ChangeListener, private juce::Slider::Listener
{
public:

    MainContentComponent()
    {
        currentIdSong = 0;
        ModeBypassed = true;
        jsonParserInit();
        addAndMakeVisible(&tableComponent);
        tableComponent.setComponentID("table");
        tableComponent.addMouseListener(this, true);

        //addAndMakeVisible(&hyperlinkButton);
        //hyperlinkButton.setButtonText("Studio-Coda.com");
        //hyperlinkButton.setURL(URL("www.studio-coda.fr"));

        addAndMakeVisible(&playerComponent);
		leftBarComponent.setState(LB_NO_CONNECTED);
        addAndMakeVisible(&leftBarComponent);
        addAndMakeVisible(&sceneComponent);


        addAndMakeVisible(&FakeCodaSlider);
        FakeCodaSlider.hideTextBox(true);
        FakeCodaSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        FakeCodaSlider.setColour(0x1001200, Colour(BACKGROUND_COLOR)); 
        FakeCodaSlider.setColour(0x1001300, Colour(THUMB_COLOR));      
        FakeCodaSlider.setColour(0x1001310, Colour(LIGNE_COLOR));
        FakeCodaSlider.addListener(this);

        addAndMakeVisible(&sceneConfigButton);
        sceneConfigButton.setButtonText("sceneConfigButton");
        sceneConfigButton.onClick = [this] { sceneConfigButtonClicked(); };
        sceneConfigButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        Image sceneConfigButtonImgOFF = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "sceneConfig.png"));

        sceneConfigButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
            sceneConfigButtonImgOFF, 1.0f, juce::Colours::transparentBlack,
            sceneConfigButtonImgOFF, 1.0f, juce::Colours::red,
            sceneConfigButtonImgOFF, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );

        addAndMakeVisible(&bypass3DButton);
        bypass3DButton.setButtonText("bypass3DButton");
        bypass3DButton.onClick = [this] { bypass3DButtonClicked(); };
        bypass3DButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        Image bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "3dbuttonOFF.png"));

        bypass3DButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
            bypass3DButtonImg, 1.0f, juce::Colours::transparentBlack,
            bypass3DButtonImg, 1.0f, juce::Colours::red,
            bypass3DButtonImg, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );

        setSize(1300, 800);
    }

    void mouseDoubleClick(const MouseEvent& event)
    {
        if(1)
        {
            if (currentIdSong != tableComponent.getRowId()  || playerComponent.Master_isPlaying()==false)
            {
                currentIdSong = tableComponent.getRowId();
                playerComponent.Master_loadAndPlay(currentIdSong);
                sceneComponent.event_notify(0);
            }
        }
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }


    void sceneConfigButtonClicked(void)
    {
        //open new windows component
        dw = new WindowSceneConfig();
        dw->setVisible(true);
		dw->setOpaque(true);
		dw->addToDesktop(ComponentPeer::windowHasTitleBar |
			ComponentPeer::windowAppearsOnTaskbar |
			ComponentPeer::windowHasMinimiseButton |
			ComponentPeer::windowHasCloseButton, 0); //StyleFlags
        dw->setAlwaysOnTop(true);   
		dw->setCentreRelative(0.5, 0.5);         
    }

    void bypass3DButtonClicked(void)
    {
        Image bypass3DButtonImg;
        if(ModeBypassed)
        {
            bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "3dbuttonON.png"));
            ModeBypassed = false;
            sceneComponent.event_notify(1);
        }
        else
        {
            bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "3dbuttonOFF.png"));
            ModeBypassed = true;
        }

            bypass3DButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
                bypass3DButtonImg, 1.0f, juce::Colours::transparentBlack,
                bypass3DButtonImg, 1.0f, juce::Colours::transparentBlack,
                bypass3DButtonImg, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
            );
        resized();
    }

    void resized() override
    {
        int Height = getHeight();
        int Width = getWidth();
        int h_playbar = 100; //largeur playbar like spotify
        
        int l_leftbar = 0.225f*Width; //300 static
        if(l_leftbar < 200)
        {
            l_leftbar = 200;
        }
        //hyperlinkButton.setBounds(10, 50,100, 20);


        leftBarComponent.setBounds(0,0, l_leftbar, Height - h_playbar);
        playerComponent.setBounds(0, Height - h_playbar, Width, h_playbar);

        sceneComponent.setBounds(l_leftbar,0,Width-l_leftbar, 0.4f * Height); //1035x50
        tableComponent.setBounds(l_leftbar - 8,  0.4f * Height - 10,   Width-l_leftbar +17,    (1 - 0.4f) * Height   - h_playbar + 20);
        FakeCodaSlider.setBounds(100,Height - 0.5*h_playbar, 200,20);


        sceneConfigButton.setBounds(0.8f * Width - 2 * ICON_BUTTON_SIZE, Height - 0.5f * (h_playbar + ICON_BUTTON_SIZE), ICON_BUTTON_SIZE, ICON_BUTTON_SIZE);
        bypass3DButton.setBounds(0.8f*Width,  Height - 0.5f * (h_playbar+ICON_BUTTON_SIZE), ICON_BUTTON_SIZE, ICON_BUTTON_SIZE);
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
    }

    void releaseResources() override
    {
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {

    }

    void sliderValueChanged(Slider* slider)
    {
        if (slider == &FakeCodaSlider)
        {
            playerComponent.setAngle(slider->getValue()/10); //0..1
        }
    }

private:


    int currentIdSong;
    bool ModeBypassed;
    //todo boutton BUY PRESQUEL

    //juce::HyperlinkButton hyperlinkButton;

    SceneComponent   sceneComponent;
    TableComponent   tableComponent;
    PlayerComponent  playerComponent;
    LeftBarComponent leftBarComponent;

    

	WindowSceneConfig* dw = 0; //config scene
    Slider FakeCodaSlider;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;






    juce::ImageButton sceneConfigButton;
    juce::ImageButton bypass3DButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
