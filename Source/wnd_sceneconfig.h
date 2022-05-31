#pragma once
#ifndef WND_SCENE_CFG_H
#define WND_SCENE_CFG_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"
#include "Path.h"
#include "polarPlan.h"
#include "sceneconfig.h"
#include "JsonParser.h"
#include "polarPlan.h"

using namespace juce;


typedef std::function<void (float)> WndConfigCallback;

class WindowSceneConfig  : public Component 
{
public:

    WindowSceneConfig(int sIDX)
    {
        IDX = sIDX;
        sceneconfig_load(&tempSceneconfig, IDX);
        addAndMakeVisible(BUTTON_OK);
        BUTTON_OK.setVisible(true);
        BUTTON_OK.setButtonText("OK");
        BUTTON_OK.onClick = [this] {ButtonOK_callback();};
        addAndMakeVisible(BUTTON_CANCEL);
        BUTTON_CANCEL.setVisible(true);
        BUTTON_CANCEL.setButtonText("Cancel");
        BUTTON_CANCEL.onClick = [this] {ButtonCANCEL_callback();};

        addAndMakeVisible(ComboAmbiant);
        addAndMakeVisible(ComboMode);
        addAndMakeVisible(ComboLiveMode);
        for(int ii = 0; ii < E_SCENE_MODE_ENUM; ii++){ComboMode.addItem(sceneconfig_buttonText(0,ii) ,ii+1);}
        for(int ii = 0; ii < E_SCENE_AMBIANT_ENUM; ii++){ComboAmbiant.addItem(sceneconfig_buttonText(1,ii) ,ii+1);}
        for(int ii = 0; ii < E_SCENE_LIVE_MODE_ENUM; ii++){ComboLiveMode.addItem(sceneconfig_buttonText(2,ii) ,ii+1);}
        ComboAmbiant.onChange = [this] {update(); };
        ComboMode.onChange = [this] {update(); };
        ComboLiveMode.onChange = [this] {update(); };
        ComboModeInstrumentSelector.onChange =  [this] {update(); };
        ComboLiveModeInstrumentSelector.onChange =  [this] {update(); };

        addAndMakeVisible(ComboModeInstrumentSelector);
        addAndMakeVisible(ComboLiveModeInstrumentSelector);
        for(int ii = 0; ii < jsonParserGetStemNumber(IDX); ii++)
        {
            ComboModeInstrumentSelector.addItem(jsonParserGetStemName(IDX,ii) ,ii+1);
            ComboLiveModeInstrumentSelector.addItem(jsonParserGetStemName(IDX,ii) ,ii+1);
        }

        ComboModeInstrumentSelector.setSelectedId(1, dontSendNotification);
        ComboLiveModeInstrumentSelector.setSelectedId(1, dontSendNotification);   

        if(sceneconfig_should_pickinstr(tempSceneconfig.mode)){ComboModeInstrumentSelector.setVisible(true); }
        else{ComboModeInstrumentSelector.setVisible(false);}
        if(tempSceneconfig.livemode){ComboLiveModeInstrumentSelector.setVisible(true);}
        else{ComboLiveModeInstrumentSelector.setVisible(false);}
        ComboAmbiant.setSelectedId(tempSceneconfig.ambiant+1,dontSendNotification);   
        ComboMode.setSelectedId(tempSceneconfig.mode+1,dontSendNotification);
        ComboLiveMode.setSelectedId(tempSceneconfig.livemode+1, dontSendNotification);
        ComboModeInstrumentSelector.setSelectedId(tempSceneconfig.mode_instrument+1,dontSendNotification);
        ComboLiveModeInstrumentSelector.setSelectedId(tempSceneconfig.livemodeInstrument + 1, dontSendNotification);

        addAndMakeVisible(polarPlanComponent);


        setLookAndFeel(&customLookAndFeel);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(&customLookAndFeel);
        }
        
        setSize(600, 700);
        setOpaque(true);
        update();
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
        update();
        wndConfigCallback(0);
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

        g.drawFittedText(sceneconfig_text_mode(IDX), Rectangle<int>(250, 350, 300,70),  juce::Justification::left, 10,1 );
        g.drawFittedText(sceneconfig_text_ambiant(IDX), Rectangle<int>(250, 450, 300,70),  juce::Justification::left, 10,1 );
        g.drawFittedText(sceneconfig_text_live(IDX), Rectangle<int>(250, 550, 300,70),  juce::Justification::left, 10,1 );

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
        ComboAmbiant.setBounds(70,     Y_ref+100,                   110,BUTTON_Y);
        ComboLiveMode.setBounds(70, Y_ref+200,                      110,BUTTON_Y);
        ComboModeInstrumentSelector.setBounds(70,Y_ref+50,     110,0.8*BUTTON_Y);
        ComboLiveModeInstrumentSelector.setBounds(70, Y_ref+250,     110,BUTTON_Y);
    }


    void setCallback(WndConfigCallback cb)
    {
       wndConfigCallback  = cb;
    }

private:

    void update(void )
    {   
        tempSceneconfig.ambiant = (E_SCENE_AMBIANT) (ComboAmbiant.getSelectedId() - 1);
        tempSceneconfig.mode_instrument = (ComboModeInstrumentSelector.getSelectedId() - 1);
        tempSceneconfig.livemodeInstrument = (E_SCENE_LIVEMODE) (ComboLiveModeInstrumentSelector.getSelectedId() - 1);
        tempSceneconfig.mode = (E_SCENE_MODE) (ComboMode.getSelectedId() - 1);
        tempSceneconfig.livemode = (E_SCENE_LIVEMODE) (ComboLiveMode.getSelectedId() - 1);
        if(sceneconfig_should_pickinstr(tempSceneconfig.mode))
        {
            ComboModeInstrumentSelector.setVisible(true);
        }
        else
        {
            ComboModeInstrumentSelector.setVisible(false);
        }

        if(tempSceneconfig.livemode)
        {
            ComboLiveModeInstrumentSelector.setVisible(true);
        }
        else
        {
            ComboLiveModeInstrumentSelector.setVisible(false);
        }

        for(int ii=0; ii<MAX_INSTR; ii++)
        {
            tempSceneconfig.slots[ii] = E_SLOT_UNUSED;
        }
        if(jsonParserGetStemNumber(IDX)==4)
        {
            switch(tempSceneconfig.mode)
            {
                     case E_SCENE_MODE_A : 
                     {
                             tempSceneconfig.slots[0] = E_SLOT_MIDLE_LLEFT;
                             tempSceneconfig.slots[1] = E_SLOT_MIDLE_LEFT;
                             tempSceneconfig.slots[2] = E_SLOT_MIDLE_RIGHT;
                             tempSceneconfig.slots[3] = E_SLOT_MIDLE_RRIGHT;
                             tempSceneconfig.slots[4] = E_SLOT_MIDLE_CENTER;
                         break;
                     }
                     case E_SCENE_MODE_B : 
                     {
                             tempSceneconfig.slots[1] = E_SLOT_MIDLE_LLEFT;
                             tempSceneconfig.slots[0] = E_SLOT_MIDLE_LEFT;
                             tempSceneconfig.slots[3] = E_SLOT_MIDLE_RIGHT;
                             tempSceneconfig.slots[2] = E_SLOT_MIDLE_RRIGHT;
                         break;
                     }
                     case E_SCENE_MODE_C : 
                     {
                             tempSceneconfig.slots[3] = E_SLOT_MIDLE_LLEFT;
                             tempSceneconfig.slots[2] = E_SLOT_MIDLE_LEFT;
                             tempSceneconfig.slots[0] = E_SLOT_MIDLE_RIGHT;
                             tempSceneconfig.slots[1] = E_SLOT_MIDLE_RRIGHT;                
                         break;
                     }
                     case E_SCENE_MODE_ONE_CENTER :     
                     {
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_CENTER;
                         break;
                     }
                     case E_SCENE_MODE_ONE_LEFT :   
                     {
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_LEFT;
                         break;
                     }
                     case E_SCENE_MODE_ONE_RIGHT : 
                     { 
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_RIGHT;
                         break;
                     }
            }
        }
        if(jsonParserGetStemNumber(IDX)==5)
        {
            switch(tempSceneconfig.mode)
            {
                     case E_SCENE_MODE_A : 
                     {
                             tempSceneconfig.slots[0] = E_SLOT_MIDLE_LLEFT;
                             tempSceneconfig.slots[1] = E_SLOT_MIDLE_LEFT;
                             tempSceneconfig.slots[2] = E_SLOT_MIDLE_RIGHT;
                             tempSceneconfig.slots[3] = E_SLOT_MIDLE_RRIGHT;
                             tempSceneconfig.slots[4] = E_SLOT_MIDLE_CENTER;
                         break;
                     }
                     case E_SCENE_MODE_B : 
                     {

                         break;
                     }
                     case E_SCENE_MODE_C : 
                     {
               
                         break;
                     }
                     case E_SCENE_MODE_ONE_CENTER :     
                     {
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_CENTER;
                         break;
                     }
                     case E_SCENE_MODE_ONE_LEFT :   
                     {
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_LEFT;
                         break;
                     }
                     case E_SCENE_MODE_ONE_RIGHT : 
                     { 
                         tempSceneconfig.slots[tempSceneconfig.mode_instrument] = E_SLOT_FRONT_RIGHT;
                         break;
                     }
            }
        }

    
        sceneconfig_save(tempSceneconfig, IDX);
        polarPlanComponent.replot(tempSceneconfig,IDX);        
        repaint();
    }



    TextButton BUTTON_OK;
    TextButton BUTTON_CANCEL;

    ComboBox ComboMode;
    ComboBox ComboModeInstrumentSelector;
    ComboBox ComboAmbiant;
    ComboBox ComboLiveMode;
    ComboBox ComboLiveModeInstrumentSelector;

    int IDX= 0;
    //TextBox TextMode;
    //TextBox TextScene;
    //Image   ImageScene;

    PolarPlanComponent polarPlanComponent;
    CustomLookAndFeel customLookAndFeel;

    Scene_config tempSceneconfig;

	WndConfigCallback wndConfigCallback; 

    SafePointer<DialogWindow> dialogWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowSceneConfig)
};

#endif //WND_SCENE_CFG_H