#pragma once
#ifndef WND_SCENE_CFG_H
#define WND_SCENE_CFG_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "Path.h"
#include "polarPlan.h"
#include "sceneconfig.h"
using namespace juce;


class WindowSceneConfig  : public Component 
{
public:

    WindowSceneConfig()
    {
        sceneconfig_load(&tempSceneconfig);

        addAndMakeVisible(BUTTON_OK);
        BUTTON_OK.setVisible(true);
        BUTTON_OK.setButtonText("OK");
        BUTTON_OK.onClick = [this] {ButtonOK_callback();};

        addAndMakeVisible(BUTTON_CANCEL);
        BUTTON_CANCEL.setVisible(true);
        BUTTON_CANCEL.setButtonText("Cancel");
        BUTTON_CANCEL.onClick = [this] {ButtonCANCEL_callback();};

        addAndMakeVisible(polarPlanComponent);
        addAndMakeVisible(ComboAmbiant);
        addAndMakeVisible(ComboMode);
        addAndMakeVisible(ComboLiveMode);
        for(int ii = 0; ii < E_SCENE_MODE_ENUM; ii++)
        {
            ComboMode.addItem(sceneconfig_buttonText(0,ii) ,ii+1);
        }
        for(int ii = 0; ii < E_SCENE_AMBIANT_ENUM; ii++)
        {
            ComboAmbiant.addItem(sceneconfig_buttonText(1,ii) ,ii+1);
        }
        for(int ii = 0; ii < E_SCENE_LIVE_MODE_ENUM; ii++)
        {
            ComboLiveMode.addItem(sceneconfig_buttonText(2,ii) ,ii+1);
        }

        ComboAmbiant.setSelectedId(tempSceneconfig.ambiant+1,dontSendNotification);   
        ComboMode.setSelectedId(tempSceneconfig.mode+1,dontSendNotification);
        ComboLiveMode.setSelectedId(tempSceneconfig.livemode+1, dontSendNotification);
    

        //addAndMakeVisible(ComboModeInstrumentSelector);
        //ComboModeInstrumentSelector.addItem("-",1);        
        //ComboModeInstrumentSelector.addItem("Piano",2);
        //ComboModeInstrumentSelector.addItem("Violin",3);
        //ComboModeInstrumentSelector.setSelectedId(1, dontSendNotification);
//
        //addAndMakeVisible(ComboLiveModeInstrumentSelector);
        //ComboLiveModeInstrumentSelector.addItem("-",1);        
        //ComboLiveModeInstrumentSelector.addItem("Piano",2);
        //ComboLiveModeInstrumentSelector.addItem("Violin",3);
        //ComboLiveModeInstrumentSelector.setSelectedId(1, dontSendNotification);   

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
        tempSceneconfig.ambiant = (E_SCENE_AMBIANT) (ComboAmbiant.getSelectedId() - 1);
        tempSceneconfig.mode = (E_SCENE_MODE) (ComboMode.getSelectedId() - 1);
        tempSceneconfig.livemode = (E_SCENE_LIVEMODE) (ComboLiveMode.getSelectedId() - 1);
        sceneconfig_save(tempSceneconfig);
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





        //switch(ConfigModeId)
        //{
        //    case 0 : 
        //    {
        //        m = "etete";
        //        break;
        //    }
//
        //}
        //g.drawFittedText(m, Rectangle<int>(350, 340, 210,100),  juce::Justification::centred, 10,1 );
//
        //switch(ConfigSceneId)
        //{
        //    case 0 : 
        //    {
        //        m = "Explication du scene en co heth eth eth eth eturs bla bla zozornzrgzrg zrg ";
        //        break;
        //    }
//
        //}
        //g.drawFittedText(m, Rectangle<int>(350, 440, 210,100),  juce::Justification::centred, 10,1 );
//
        //switch(ConfigModeLiveMode)
        //{
        //    case 0 : 
        //    {
        //        m = "Live mode //todo chixehethethet  instru etc";
        //        break;
        //    }
//
        //}
        //g.drawFittedText(m, Rectangle<int>(350, 540, 210,100),  juce::Justification::centred, 10,1 );


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
        ComboAmbiant.setBounds(70,     Y_ref+100,                   110,BUTTON_Y);
        ComboLiveMode.setBounds(70, Y_ref+200,                      110,BUTTON_Y);
        ComboLiveModeInstrumentSelector.setBounds(70+110+20, Y_ref+200,     110,BUTTON_Y);

    }

    //void showPopupWindows(void);
	
private:

    TextButton BUTTON_OK;
    TextButton BUTTON_CANCEL;

    ComboBox ComboMode;
    ComboBox ComboModeInstrumentSelector;

    ComboBox ComboAmbiant;

    ComboBox ComboLiveMode;
    ComboBox ComboLiveModeInstrumentSelector;
    //TextBox TextMode;
    //TextBox TextScene;
    //Image   ImageScene;

    PolarPlanComponent polarPlanComponent;
    CustomLookAndFeel customLookAndFeel;

    Scene_config tempSceneconfig;

    SafePointer<DialogWindow> dialogWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowSceneConfig)
};

#endif //WND_SCENE_CFG_H