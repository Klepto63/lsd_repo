#pragma once
using namespace juce;

#include "table.h"
#include "LeftBarComponent.h"
#include "playerComponent.h"
#include "SceneComponent.h" 
#include "JsonParser.h"

//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    public juce::ChangeListener, private juce::Slider::Listener
{
public:

    MainContentComponent()
    {
        currentIdSong = 0;
        jsonParserInit();
        addAndMakeVisible(&tableComponent);
        tableComponent.setComponentID("table");
        tableComponent.addMouseListener(this, true);

        //addAndMakeVisible(&hyperlinkButton);
        //hyperlinkButton.setButtonText("Studio-Coda.com");
        //hyperlinkButton.setURL(URL("www.studio-coda.fr"));

        addAndMakeVisible(&playerComponent);
		leftBarComponent.setState(LB_PREQUEL_MODE);
        addAndMakeVisible(&leftBarComponent);
        addAndMakeVisible(&sceneComponent);


        addAndMakeVisible(&FakeCodaSlider);
        FakeCodaSlider.hideTextBox(true);
        FakeCodaSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
        FakeCodaSlider.setColour(0x1001200, Colour(BACKGROUND_COLOR)); 
        FakeCodaSlider.setColour(0x1001300, Colour(THUMB_COLOR));      
        FakeCodaSlider.setColour(0x1001310, Colour(LIGNE_COLOR));
              
        FakeCodaSlider.addListener(this);

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
            }
        }
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
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


        sceneComponent.setBounds(l_leftbar,0,Width-265, 0.4f * Height); //1035x50

        tableComponent.setBounds(l_leftbar - 8,  0.4f * Height - 10,   Width-l_leftbar +17,    (1 - 0.4f) * Height   - h_playbar + 20);

        FakeCodaSlider.setBounds(100,Height - 0.5*h_playbar, 200,20);
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
    //todo boutton BUY PRESQUEL

    //juce::HyperlinkButton hyperlinkButton;

    SceneComponent   sceneComponent;
    TableComponent   tableComponent;
    PlayerComponent  playerComponent;
    LeftBarComponent leftBarComponent;

    
    Slider FakeCodaSlider;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
