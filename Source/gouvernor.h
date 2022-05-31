#pragma once
using namespace juce;

#include "table.h"
#include "LeftBarComponent.h"
#include "playerComponent.h"
#include "SceneComponent.h" 
#include "JsonParser.h"
#include "wnd_sceneconfig.h"
#include "EnergySliderLookAndFeel.h"
#include "sceneconfig.h"
//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    public juce::ChangeListener, private juce::Slider::Listener
{
public:

    MainContentComponent()
    {
        currentIdSong = -1;
        ModeBypassed = true;
        jsonParserInit();
        sceneconfig_init();
        addAndMakeVisible(&tableComponent);
        tableComponent.setComponentID("table");
        tableComponent.addMouseListener(this, true);

        //addAndMakeVisible(&hyperlinkButton);
        //hyperlinkButton.setButtonText("Studio-Coda.com");
        //hyperlinkButton.setURL(URL("www.studio-coda.fr"));

        addAndMakeVisible(&playerComponent);
        addAndMakeVisible(&leftBarComponent);
        addAndMakeVisible(&sceneComponent);
        leftBarComponent.setletbarCallback([this](int v)
        {
            leftbarupdated(v);
        });
        sceneComponent.setSceneComponentOnChangeCallback([this](float v)
        {
            playerComponent.applyHightlight();
        });

        addAndMakeVisible(&sceneConfigButton);
        sceneConfigButton.setButtonText("sceneConfigButton");
        sceneConfigButton.onClick = [this] { sceneConfigButtonClicked(); };
        sceneConfigButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        Image sceneConfigButtonImgOFF = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_SCENE_CONFIG)));
        sceneConfigButton.setImages(true,  //re-size to fit
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
        Image bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_3DBUTTON_OFF)));

        bypass3DButton.setImages(true,  //re-size to fit
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
                playerComponent.Master_changeState(PlayerComponent::Master_Stopped);
                playerComponent.Master_loadAndPlay(currentIdSong);

                sceneComponent.updateConfig(currentIdSong);
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
        if(currentIdSong == -1)
        {
            //no song selected = no popup
            AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,TRANS("No song selected"),"Select a song to edit spatial configuration");   
            return;
        }

        bool shouldPopup = false;
		if(dw == 0)
		{
			shouldPopup = true;
		}
		else if (dw->isVisible() == false)
		{
			shouldPopup = true;
		}
		if(shouldPopup)
        {
            
            dw = new WindowSceneConfig(currentIdSong);
            dw->setName("Spatial Configuration");
            dw->setVisible(true);
		    dw->setOpaque(true);
		    dw->addToDesktop(ComponentPeer::windowHasTitleBar |
		    	ComponentPeer::windowAppearsOnTaskbar |
		    	ComponentPeer::windowHasMinimiseButton |
		    	ComponentPeer::windowHasCloseButton, 0); //StyleFlags
            dw->setAlwaysOnTop(true);   
		    dw->setCentreRelative(0.5, 0.5); 
            dw->setCallback([this](float f)
            {
                sceneComponent.updateConfig(currentIdSong);
                //playerComponent.updateConfig();
            });

        }
        
    }

    void bypass3DButtonClicked(void)
    {
        Image bypass3DButtonImg;
        if(ModeBypassed)
        {
            bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_3DBUTTON_ON)));
            ModeBypassed = false;
            sceneComponent.event_notify(1);
        }
        else
        {
            bypass3DButtonImg = ImageFileFormat::loadFrom(File::File(PathGetAsset(ASSET_3DBUTTON_OFF)));
            ModeBypassed = true;
        }

            bypass3DButton.setImages(true,  //re-size to fit
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

        sceneComponent.setBounds(l_leftbar,0,Width-l_leftbar, 0.3f * Height); //1035x50

        tableComponent.setBounds(l_leftbar - 8,  0.3f * Height - 10,   Width-l_leftbar +17,    (1 - 0.3f) * Height   - h_playbar + 20);
        //FakeCodaSlider.setBounds(100,Height - 0.5*h_playbar, 200,20);

        sceneConfigButton.setBounds(0.8f * Width - 2 * ICON_BUTTON_SIZE, Height - 0.5f * (h_playbar + ICON_BUTTON_SIZE), ICON_BUTTON_SIZE, ICON_BUTTON_SIZE);
        bypass3DButton.setBounds(0.8f*Width,  Height - 0.5f * (h_playbar+ICON_BUTTON_SIZE), ICON_BUTTON_SIZE, ICON_BUTTON_SIZE);

        int size = 190;
        energySlider.setBounds(0.5f*(l_leftbar-size),40,size,size);

        recenterButton.setBounds(0.5f*(l_leftbar - 70),160,70,30);   
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
        if (slider == &energySlider)
        {
            //playerComponent.setAngle(slider->getValue()/10); //0..1
            float f = slider->getValue()/10;  
            leftBarComponent.setAngle(f);
            playerComponent.setAngle(f);
        }        
    }

private:

    void leftbarupdated(int v)
    {
        if(v == ((LeftBarComponent::LB_FAKE_MODE)))
        {
            //todo only in mode
            addAndMakeVisible(&energySlider);
            energySlider.setSliderStyle(Slider::Rotary);
            energySlider.setValue(5, juce::dontSendNotification); //1..10
            energySlider.setLookAndFeel(&energySliderLookAndFeel);
            energySlider.hideTextBox(true);
            energySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
            energySlider.addListener(this);
            energySlider.setRotaryParameters(MathConstants<float>::pi * 1.5f, MathConstants<float>::pi * 2.5f, true);
            energySlider.setEnabled(true);

		    addAndMakeVisible(&recenterButton);
            recenterButton.setColour(juce::TextButton::buttonColourId, Colour(RECENTER_BUTTON));
            recenterButton.setColour(juce::ComboBox::outlineColourId, Colour(RECENTER_BUTTON));
		    recenterButton.setButtonText("Recenter");
        }
        else
        {

        }
    }


    int currentIdSong;
    bool ModeBypassed;
    //todo boutton BUY PRESQUEL

    //juce::HyperlinkButton hyperlinkButton;

    CustomLookAndFeel customLookAndFeel;

    SceneComponent   sceneComponent;
    TableComponent   tableComponent;
    PlayerComponent  playerComponent;

    LeftBarComponent leftBarComponent; //public for popup windows

    

	WindowSceneConfig* dw = 0; //config scene

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;


    EnergySliderLookAndFeel energySliderLookAndFeel;
    juce::Slider energySlider;


    juce::ImageButton sceneConfigButton;
    juce::ImageButton bypass3DButton;

    TextButton recenterButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
