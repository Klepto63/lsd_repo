#pragma once

#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"
#include "sceneconfig.h"
#include "customLookAndFeel.h"

typedef std::function<void (float)> SceneComponentOnChangeCallback;

class SceneComponent : public juce::AnimatedAppComponent
{

typedef enum
{
        SCENE_TUTO_INIT = 0,            // please select a song
        SCENE_TUTO_2D,                  // you can enable 3D feature by cliking on inco ([])
        SCENE_TUTO_2D_2,
        SCENE_TUTO_CONNECT,              // instruments are now in 3D place.
        SCENE_PLAYER_MODE,
}SCENE_TUTO;
    

public:

    SceneComponent()
    {

        setLookAndFeel(&customLookAndFeel);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(&customLookAndFeel);
        }

        setSize(600, 600);
        //setFramesPerSecond(1);

        sceneId = SCENE_TUTO_INIT;
        repaint();

        background  = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_3D_GRID)));


        //ImgPianoButton = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_INSTR_PIANO)));

        
        addAndMakeVisible(&skipTutorial);
        skipTutorial.setButtonText("skip tutorial");
        skipTutorial.setColour(0x1000100, Colour((uint32)SCENE_COMPONENT_WP));
        skipTutorial.setColour(0x1000102, Colour((uint32)SCENE_COMPONENT_LABEL));   //text color
        skipTutorial.onClick = [this] {sceneId = SCENE_PLAYER_MODE; repaint(); };

        for(int ii = 0; ii<MAX_INSTR; ii++)
        {
            addAndMakeVisible(&ButtonSlot[ii]);
            ButtonSlot[ii].setVisible(false);
        }

        ButtonSlot[0].onClick = [this] {buttonClicked0();};
        ButtonSlot[1].onClick = [this] {buttonClicked1();};
        ButtonSlot[2].onClick = [this] {buttonClicked2();};
        ButtonSlot[3].onClick = [this] {buttonClicked3();};
        ButtonSlot[4].onClick = [this] {buttonClicked4();};
    }



    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
    }

    void updateConfig(int IDX)
    {
        //todo clear

        Scene_config sc;
        sceneconfig_load(&sc, IDX);
        for(int ii = 0; ii < MAX_INSTR; ii++)
        {

            e_slot e = sc.slots[ii];
            if(e != E_SLOT_UNUSED)
            {
                ImageButtonSlot[ii] = ImageFileFormat::loadFrom(File::File(jsonParserGetStemImgPath(IDX, ii))); 
                ButtonActive[ii] = 1;
            }
            else
            {
                ButtonActive[ii] = 0;
            }
        
        }
        repaint();
    }

    void drawCirclePolar(juce::Graphics& g, float dist, float angle, float rad, juce::Colour color)
    {
        //Point repère : drawcircle(g, 0.5, 0.85, 40, juce::Colours::white);
        float x = 0.5 + dist * (std::cos(angle * 2 * 3.1415 / 360));
        float y = 0.5 - dist * (std::sin(angle * 2 * 3.1415 / 360));
        //float y = 0.95 - dist * (std::sin(angle * 2 * 3.1415 / 360));
        drawcircle(g, x, y, rad, color);
    }

    void drawcircle(juce::Graphics& g, float x, float y, float rad, juce::Colour color)
    {
        auto w = getLocalBounds().toFloat().getWidth();
        auto h = getLocalBounds().toFloat().getHeight();
        g.setColour(color);
        g.fillEllipse((w * x) - (rad * 0.5f), (h * y) - (rad * 0.5f), rad, rad);
    }

    void event_notify(int idx)
    {

        if((idx == 0) && (sceneId ==SCENE_TUTO_INIT))
        {
            sceneId = SCENE_TUTO_2D;
        }
        if((idx == 1) && (sceneId ==SCENE_TUTO_2D))
        {
            sceneId = SCENE_TUTO_CONNECT;
        }   
        repaint();
    }


    void drawScene(juce::Graphics& g)
    {
        g.setColour(juce::Colours::black);
        //void Graphics::drawImage (const Image& imageToDraw, Rectangle<float> targetArea,
        //                          RectanglePlacement placementWithinTarget, bool fillAlphaChannelWithCurrentBrush) const
        //
        //
        //g.drawImageAt(background, 0,0,false);
        //g.drawImage(instrument1, 485,280,   95,95,   0,0,400,400,false);

//void Graphics::drawImage (const Image& imageToDraw,
//                          int dx, int dy, int dw, int dh,
//                          int sx, int sy, int sw, int sh,
//                          const bool fillAlphaChannelWithCurrentBrush) const
//
        //auto w = getLocalBounds().toFloat().getWidth();
        //auto h = getLocalBounds().toFloat().getHeight();
        //g.drawLine(0, 0.5*h, w, 0.5*h);
        //g.drawLine(0.5*w, 0, 0.5*w, h);
        //g.drawLine(0, 0, w, h);
        //DESSIN TRAITS RONDS
        //g.drawLine(w, 0, 0, h);
        //float rad;
        //for (int i = 1; i < 5; i++)
        //{
        //    rad = 0.3 * i;
        //    g.drawEllipse(0.5 * w - (rad * 0.5 * w), 0.5 * h - (rad * 0.5 * h), rad * w, rad * h, 3 - 0.5 * i);
        //}
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        //g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));
        String m;
        int i =0;
        static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
        Font Gofont = Font(typeface);        
        switch(sceneId)
        {
            case SCENE_TUTO_INIT : 
            {
                g.fillAll(Colour((uint32)SCENE_COMPONENT_WP));
                g.setColour(Colour((uint32)P1_COLOR));
                g.setFont(CODAFRONT_TEXT_SIZE_P1);
                Gofont.setHeight(26);
	            g.drawText ("Welcome to Coda demo player", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
	            m << "Let's pick a song";	
                Gofont.setHeight(18);
                g.setFont(Gofont);
                g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
                break;
            }
            case SCENE_TUTO_2D :
            {
                g.fillAll(Colour((uint32)WP1_COLOR));
                g.setColour(Colour((uint32)P1_COLOR));
                g.setFont(CODAFRONT_TEXT_SIZE_P1);
                Gofont.setHeight(26);
    		    //g.drawText ("Enable 3D audio", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		    m << "Activate Audio 360 by clicking on the icon. TODO Control panel ";	
                Gofont.setHeight(18);
                g.setFont(Gofont);
                g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
                break;
            }
            case SCENE_TUTO_2D_2 :
            {
                g.fillAll(Colour((uint32)WP1_COLOR));
                g.setColour(Colour((uint32)P1_COLOR));
                g.setFont(CODAFRONT_TEXT_SIZE_P1);
                Gofont.setHeight(26);
    		    //g.drawText ("Enable 3D audio", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		    m << "use the control button to edit spatial configuration ";	
                Gofont.setHeight(18);
                g.setFont(Gofont);
                g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
                break;
            }
            case SCENE_TUTO_CONNECT :
            {
                g.fillAll(Colour((uint32)WP1_COLOR));
                g.setColour(Colour((uint32)P1_COLOR));
                g.setFont(CODAFRONT_TEXT_SIZE_P1);
                Gofont.setHeight(26);
    		    //g.drawText ("Enable 3D audio", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		    m << "We can now connect the headtracker for full experience. If you don't have one, you can buy one at. Meanwhile, simulate by";	
                Gofont.setHeight(18);
                g.setFont(Gofont);
                g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
                break;
            }
            case SCENE_PLAYER_MODE : 
            {
                skipTutorial.setVisible(false);
                int h = getHeight();
                int w = getWidth();
                g.drawImage(background,
                    0, 0,    //dest
                    w, h,
                    0, 0,    //img
                    background.getWidth(),
                    background.getHeight(),
                    false);
                int highglightmode = -1;
                float opacity = 0.50f;    
                for(int ii = 0; ii < MAX_INSTR; ii++)
                {
                    if(ButtonActive[ii] == 2)
                    {
                        highglightmode = ii;
                    }
                }
                for(int ii = 0; ii < MAX_INSTR; ii++)
                {
                    if (ButtonActive[ii])
                    {
                        if(highglightmode != -1)
                        {
                            opacity = ButtonActive[ii] == 2 ? 1.0f : 0.40f;    
                        }
                        else 
                        {
                            opacity = 0.90f;
                        }
                        ButtonSlot[ii].setImages(false,
                            true,
                            true,
                            ImageButtonSlot[ii], opacity, juce::Colours::transparentBlack,
                            ImageButtonSlot[ii], 1.00f, juce::Colours::transparentBlack,
                            ImageButtonSlot[ii], 1.00f, juce::Colours::transparentBlack,
                            0.5f);
                        ButtonSlot[ii].setVisible(true);
                    }
                    else
                    {
                        ButtonSlot[ii].setVisible(false);
                    }
                }

                //addAndMakeVisible(&PianoButton);
                //PianoButton.setButtonText("Piano");
                //PianoButton.setImages(false,  //resize to fit
                //    true,  //rescale image
                //    true,  //preserve proportion
                //    ImgPianoButton, 0.90f, juce::Colours::transparentBlack, 
                //    ImgPianoButton, 1.0f, juce::Colours::transparentBlack, // over
                //    ImgPianoButton, 1.0f, juce::Colours::transparentBlack, // clicked
                //    0.5f
                //);
//
                //addAndMakeVisible(&ViolinButton);
                //ViolinButton.setButtonText("Violin");
                //ViolinButton.setImages(false,  //resize to fit
                //    true,  //rescale image
                //    true,  //preserve proportion
                //    ImgViolinButton, 0.90f, juce::Colours::transparentBlack,
                //    ImgViolinButton, 1.0f, juce::Colours::transparentBlack,
                //    ImgViolinButton, 1.0f, juce::Colours::transparentBlack, //image when down
                //    0.5f
                //);

                break;
            }
            default : 
            {
                break;
            }
        }
    }


        //drawScene(g);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter(), 30, juce::Colours:: blue);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 45, 30                              , juce::Colours::purple);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 90, 30                              , juce::Colours::red);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 135, 30                             , juce::Colours::yellow);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 180, 30                             , juce::Colours::green);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 225, 30                             , juce::Colours::yellowgreen);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 270, 30                             , juce::Colours::pink);
        //drawCirclePolar(g, 0.3f, (float) getFrameCounter() + 315, 30                             , juce::Colours::fuchsia);

        /*
        auto fishLength = 10;
        juce::Path spinePath;
        for (auto i = 0; i < fishLength; ++i)
        {
            auto radius = 100 + 10 * std::sin((float)getFrameCounter() * 0.1f + (float)i * 0.5f);
            g.setColour(juce::Colours::red);
            juce::Point<float> p((float)getWidth() / 2.0f + 1.5f * (float)radius * std::sin((float)getFrameCounter() * 0.02f + (float)i * 0.12f),
                (float)getHeight() / 2.0f + 1.0f * (float)radius * std::cos((float)getFrameCounter() * 0.04f + (float)i * 0.12f));
            g.fillEllipse(p.x - (float)i, p.y - (float)i, 2.0f + 2.0f * (float)i, 2.0f + 2.0f * (float)i);     // [1]
            if (i == 0)
                spinePath.startNewSubPath(p);  
            else
                spinePath.lineTo(p);           
        }
                //g.strokePath(spinePath, juce::PathStrokeType(4.0f));
        */
    

    void resized() override
    {
        int Height = getHeight();
        int Width = getWidth();
        int BUTTON_WIDTH = 100;
        int BUTTON_HEIGHT = 30;

        skipTutorial.setBounds(Width - BUTTON_WIDTH - 20 ,Height - BUTTON_HEIGHT - 20 ,BUTTON_WIDTH,BUTTON_HEIGHT);

        int lbox = 0.7*Height;
        for(int ii = 0; ii < MAX_INSTR; ii++)
        {
            if(ButtonActive[ii])
            {
                ButtonSlot[ii].setBounds(100.0f*ii+(Width-lbox)/2 , Height/6,lbox,lbox);
            }
        }
    }

    void setSceneComponentOnChangeCallback(SceneComponentOnChangeCallback cb) { sceneComponentOnChangeCallback = cb; };

private:

    void buttonClicked0()  
    {
        if(ButtonActive[0] == 2)
        {
            ButtonActive[0] = 1;
        }
        else if(ButtonActive[0] == 1)
        {
            for(int ii = 0; ii < MAX_INSTR ; ii++)
            {
                if(ButtonActive[ii] == 2)
                {
                    ButtonActive[ii] = 1;
                }
            }                
            ButtonActive[0] = 2;
        }
        repaint();
        sceneconfig_save_highlight(ButtonActive);        
        sceneComponentOnChangeCallback(0);        
    }
    void buttonClicked1()  
    {
        if(ButtonActive[1] == 2)
        {
            ButtonActive[1] = 1;
        }
        else if(ButtonActive[1] == 1)
        {
            for(int ii = 0; ii < MAX_INSTR ; ii++)
            {
                if(ButtonActive[ii] == 2)
                {
                    ButtonActive[ii] = 1;
                }
            }                
            ButtonActive[1] = 2;
        }
        repaint();
        sceneconfig_save_highlight(ButtonActive);        
        sceneComponentOnChangeCallback(0);        
    }
    void buttonClicked2()  
    {
        if(ButtonActive[2] == 2)
        {
            ButtonActive[2] = 1;
        }
        else if(ButtonActive[2] == 1)
        {
            for(int ii = 0; ii < MAX_INSTR ; ii++)
            {
                if(ButtonActive[ii] == 2)
                {
                    ButtonActive[ii] = 1;
                }
            }                
            ButtonActive[2] = 2;
        }
        repaint();
        sceneconfig_save_highlight(ButtonActive);        
        sceneComponentOnChangeCallback(0);        
    }
    void buttonClicked3()  
    {
        if(ButtonActive[3] == 2)
        {
            ButtonActive[3] = 1;
        }
        else if(ButtonActive[3] == 1)
        {
            for(int ii = 0; ii < MAX_INSTR ; ii++)
            {
                if(ButtonActive[ii] == 2)
                {
                    ButtonActive[ii] = 1;
                }
            }                
            ButtonActive[3] = 2;
        }
        repaint();
        sceneconfig_save_highlight(ButtonActive);        
        sceneComponentOnChangeCallback(0);        
    }
    void buttonClicked4()  
    {
        if(ButtonActive[4] == 2)
        {
            ButtonActive[4] = 1;
        }
        else if(ButtonActive[3] == 1)
        {
            for(int ii = 0; ii < MAX_INSTR ; ii++)
            {
                if(ButtonActive[ii] == 2)
                {
                    ButtonActive[ii] = 1;
                }
            }                
            ButtonActive[4] = 2;
        }



        repaint();
        sceneconfig_save_highlight(ButtonActive);
        sceneComponentOnChangeCallback(0);
    }






    int sceneId=0;
    TextButton skipTutorial;
    TextButton purchaseButton;
    
    Image background;

    ImageButton ButtonSlot[MAX_INSTR];
    Image       ImageButtonSlot[MAX_INSTR];
    int         ButtonActive[MAX_INSTR]; //0(off)..1(active)..2(highlight) 

    CustomLookAndFeel customLookAndFeel;
    SceneComponentOnChangeCallback sceneComponentOnChangeCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)

};//scene componenet.h


#endif //SCENE_COMPONENT_H