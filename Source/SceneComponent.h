#pragma once

#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"

#include "customLookAndFeel.h"

class SceneComponent : public juce::AnimatedAppComponent
{

typedef enum
{
        SCENE_TUTO_INIT = 0,            // please select a song
        SCENE_TUTO_2D,                  // you can enable 3D feature by cliking on inco ([])
        SCENE_TUTO_CONNECT,              // instruments are now in 3D place.
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
        setFramesPerSecond(1);

        sceneId = SCENE_TUTO_INIT;
        load_scene();


        addAndMakeVisible(&skipTutorial);
        skipTutorial.setButtonText("skip tutorial");
        skipTutorial.setColour(0x1000100, Colour((uint32)SCENE_COMPONENT_WP));
        skipTutorial.setColour(0x1000102, Colour((uint32)SCENE_COMPONENT_LABEL));   //text color

        load_init_scene();


    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.


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

    void load_background()
    {
        //background  = ImageFileFormat::loadFrom(File::File(PathGetAsset(PATH_SONG) + (juce::String)"/song1/scene/1.png" ));
        //instrument1 = ImageFileFormat::loadFrom(File::File("C:/Users/Alex/Desktop/Coda2022/songs/raw_/harpi_a.png"));
    }


    void event_notify(int idx)
    {

        if((idx == 0) && (sceneId ==SCENE_TUTO_INIT))
        {
            sceneId = SCENE_TUTO_2D;
            load_scene();
        }
        if((idx == 1) && (sceneId ==SCENE_TUTO_2D))
        {
            sceneId = SCENE_TUTO_CONNECT;
            load_scene();
        }   

    }


    void load_init_scene()
    {

    }

    void load_scene()
    {
        //load_background();
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
        //g.fillAll(juce::Colours::black);
        //g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));

        String m;
        int i =0;
        switch(sceneId)
        {
            case SCENE_TUTO_INIT : 
            {
                g.fillAll(Colour((uint32)SCENE_COMPONENT_WP));
                g.setColour(Colour((uint32)P1_COLOR));
                g.setFont(CODAFRONT_TEXT_SIZE_P1);
                static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
                Font Gofont = Font(typeface);
                Gofont.setHeight(26);
	            g.drawText ("Welcome to Coda demo player", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
	            m << "Let's start by selecting a song..";	
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
                static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
                Font Gofont = Font(typeface);
                Gofont.setHeight(26);
    		    //g.drawText ("Enable 3D audio", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		    m << "you can now enable 3D audio by clicling on the icone 2D";	
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
                static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
                Font Gofont = Font(typeface);
                Gofont.setHeight(26);
    		    //g.drawText ("Enable 3D audio", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		    m << "Now connect presque. If you don't have one, you can buy one at. Meanwhile, simulate by";	
                Gofont.setHeight(18);
                g.setFont(Gofont);
                g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
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
    }

private:

    int sceneId=0;
    TextButton skipTutorial;
    //Image background;
    //Image instrument1;

    CustomLookAndFeel customLookAndFeel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)

};//scene componenet.h


#endif //SCENE_COMPONENT_H