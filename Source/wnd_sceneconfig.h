#pragma once
#ifndef WND_SCENE_CFG_H
#define WND_SCENE_CFG_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "Path.h"
#include "polarPlan.h"
using namespace juce;


class WindowSceneConfig  : public Component 
{
public:

    WindowSceneConfig()
    {
        addAndMakeVisible(BUTTON_OK);
        BUTTON_OK.setVisible(true);
        BUTTON_OK.setButtonText("OK");
        BUTTON_OK.onClick = [this] {ButtonOK_callback();};

        addAndMakeVisible(BUTTON_CANCEL);
        BUTTON_CANCEL.setVisible(true);
        BUTTON_CANCEL.setButtonText("Cancel");
        BUTTON_CANCEL.onClick = [this] {ButtonCANCEL_callback();};

        addAndMakeVisible(polarPlanComponent);
        
        addAndMakeVisible(ComboMode);
        ComboMode.addItem("Default",1);
        ComboMode.addItem("Far",2);
        ComboMode.addItem("Duo only",3);
        ComboMode.addItem("One instrument in front",4);        
        ComboMode.addItem("Tournant",5);        
        ComboMode.setSelectedId(1,dontSendNotification);

        addAndMakeVisible(ComboSceneEQ);
        ComboSceneEQ.addItem("Natural",1);
        ComboSceneEQ.addItem("Opera",2);
        ComboSceneEQ.addItem("Space",3);
        ComboSceneEQ.addItem("Dry",4);        
        ComboSceneEQ.setSelectedId(1,dontSendNotification);        

        addAndMakeVisible(ComboLiveMode);
        ComboLiveMode.addItem("Disabled",1);
        ComboLiveMode.addItem("Enabled",2);
        ComboLiveMode.setSelectedId(1, dontSendNotification);

        addAndMakeVisible(ComboModeInstrumentSelector);
        ComboModeInstrumentSelector.addItem("-",1);        
        ComboModeInstrumentSelector.addItem("Piano",2);
        ComboModeInstrumentSelector.addItem("Violin",3);
        ComboModeInstrumentSelector.setSelectedId(1, dontSendNotification);

        addAndMakeVisible(ComboLiveModeInstrumentSelector);
        ComboLiveModeInstrumentSelector.addItem("-",1);        
        ComboLiveModeInstrumentSelector.addItem("Piano",2);
        ComboLiveModeInstrumentSelector.addItem("Violin",3);
        ComboLiveModeInstrumentSelector.setSelectedId(1, dontSendNotification);   

        setLookAndFeel(&customLookAndFeel);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(&customLookAndFeel);
        }
        
        setSize(600, 700);
        setOpaque(true);
    }
    ~WindowSceneConfig() override
    {
        setLookAndFeel(nullptr);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(nullptr);
        }
    }


    void WindowSceneConfig::ButtonOK_callback(void)
    {
        delete this;
    }
    void WindowSceneConfig::ButtonCANCEL_callback(void)
    {
           delete this;
    }

    void paint (juce::Graphics& g) override
    {
        int h = getHeight();
        int w = getWidth();
        String m;
	    g.fillAll(Colour(WP_LEFTBAR));

        g.setColour(Colour((uint32)P1_COLOR));
        m = "Mode";
        //g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
        g.drawFittedText(m, Rectangle<int>(25, 350, 70,70),  juce::Justification::left, 10,1 );

        m = "Scene";
        g.drawFittedText(m, Rectangle<int>(25, 450, 70,70),  juce::Justification::left, 10,1 );

        m = "Live";
        g.drawFittedText(m, Rectangle<int>(25, 550, 70,70),  juce::Justification::left, 10,1 );       



        switch(ConfigModeId)
        {
            case 0 : 
            {
                m = "Explication du mode en courethetheth s bla bla zozornzrgzrg zrg ";
                break;
            }

        }
        g.drawFittedText(m, Rectangle<int>(350, 340, 210,100),  juce::Justification::centred, 10,1 );

        switch(ConfigSceneId)
        {
            case 0 : 
            {
                m = "Explication du scene en co heth eth eth eth eturs bla bla zozornzrgzrg zrg ";
                break;
            }

        }
        g.drawFittedText(m, Rectangle<int>(350, 440, 210,100),  juce::Justification::centred, 10,1 );

        switch(ConfigModeLiveMode)
        {
            case 0 : 
            {
                m = "Live mode //todo chixehethethet  instru etc";
                break;
            }

        }
        g.drawFittedText(m, Rectangle<int>(350, 540, 210,100),  juce::Justification::centred, 10,1 );


    }

    void resized() override
    {   
        int Height = getHeight();//800
        int Width = getWidth();//600
        int BUTTON_X = 65;
        int BUTTON_Y = 35;
    
        BUTTON_CANCEL.setBounds(Width - 1.3 * BUTTON_X ,Height - 50,BUTTON_X,BUTTON_Y);
        BUTTON_OK.setBounds(Width - 2.6 * BUTTON_X ,Height - 50,BUTTON_X,BUTTON_Y);
        polarPlanComponent.setBounds(25,25,Width-25-25,300);  //limite polar plan = 300 + 25

        //ref 
        int Y_ref = 368;

        ComboMode.setBounds(70,  Y_ref,                             110,BUTTON_Y);
        ComboModeInstrumentSelector.setBounds(70+110+20, Y_ref,     110,BUTTON_Y);
        ComboSceneEQ.setBounds(70,     Y_ref+100,                   110,BUTTON_Y);
        ComboLiveMode.setBounds(70, Y_ref+200,                      110,BUTTON_Y);
        ComboLiveModeInstrumentSelector.setBounds(70+110+20, Y_ref+200,     110,BUTTON_Y);

    }

    //void showPopupWindows(void);
	
private:

    TextButton BUTTON_OK;
    TextButton BUTTON_CANCEL;

    ComboBox ComboMode;
    ComboBox ComboModeInstrumentSelector;

    ComboBox ComboSceneEQ;

    ComboBox ComboLiveMode;
    ComboBox ComboLiveModeInstrumentSelector;
    //TextBox TextMode;
    //TextBox TextScene;
    //Image   ImageScene;

    PolarPlanComponent polarPlanComponent;
    CustomLookAndFeel customLookAndFeel;

    //config to struct
    int ConfigSceneId = 0;
    int ConfigModeId  = 0;
    int ConfigModeLiveMode = 0;
    SafePointer<DialogWindow> dialogWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowSceneConfig)
};

#endif //WND_SCENE_CFG_H