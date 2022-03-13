#pragma once
using namespace juce;

#include "table.h"
#include "LeftBarComponent.h"
#include "playerComponent.h"
#include "SceneComponent.h" 
#include "JsonParser.h"

//==============================================================================
class MainContentComponent : public juce::AudioAppComponent,
    public juce::ChangeListener
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


        setSize(1300, 800);
    }

    void mouseDoubleClick(const MouseEvent& event)
    {
        if(1)
        {
            if (currentIdSong != tableComponent.getRowId()  || playerComponent.isPlaying()==false)
            {
                currentIdSong = tableComponent.getRowId();
                playerComponent.loadAndPlay(currentIdSong);
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
        int h_table = 200;
        int l_leftbar = 300;
        //hyperlinkButton.setBounds(10, 50,100, 20);
        playerComponent.setBounds(0, Height - h_playbar, Width, h_playbar);
        tableComponent.setBounds(l_leftbar - 8, Height - h_playbar - h_table, Width-l_leftbar +17, 210);
        leftBarComponent.setBounds(0,0, l_leftbar, Height - h_playbar);
        sceneComponent.setBounds(l_leftbar,0,Width-265, Height - h_playbar - 192); //1035x50


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



private:

    int currentIdSong;
    //todo boutton BUY PRESQUEL

    //juce::HyperlinkButton hyperlinkButton;

    SceneComponent   sceneComponent;
    TableComponent   tableComponent;
    PlayerComponent  playerComponent;
    LeftBarComponent leftBarComponent;

    
 

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};