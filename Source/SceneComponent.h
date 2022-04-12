#pragma once

#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"


class SceneComponent : public juce::AnimatedAppComponent
{
public:

    SceneComponent()
    {
        setSize(600, 600);
        load_background();
        setFramesPerSecond(10); //??

        addAndMakeVisible(&sceneConfigButton);
        sceneConfigButton.setButtonText("sceneConfigButton");
        sceneConfigButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        Image sceneConfigButtonImg = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "3dbutton.png"));
        Image sceneConfigButtonImg2 = ImageFileFormat::loadFrom(File::File(ABS_PATH_ASSETS + (juce::String) "3dbutton2.png"));
        sceneConfigButton.setImages(true,  //resize to fit
                             true,  //rescale image
                             true,  //preserve proportion
            sceneConfigButtonImg, 1.0f, juce::Colours::transparentBlack,
            sceneConfigButtonImg, 1.0f, juce::Colours::white,
            sceneConfigButtonImg2, 1.0f, juce::Colours::transparentBlack, //image when down
            0.5f
        );
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
        background  = ImageFileFormat::loadFrom(File::File(ABS_PATH_SONGS + (juce::String)"/song1/scene/1.png" ));
        //instrument1 = ImageFileFormat::loadFrom(File::File("C:/Users/Alex/Desktop/Coda2022/songs/raw_/harpi_a.png"));
    }

    void drawScene(juce::Graphics& g)
    {
        g.setColour(juce::Colours::black);

        //void Graphics::drawImage (const Image& imageToDraw, Rectangle<float> targetArea,
        //                          RectanglePlacement placementWithinTarget, bool fillAlphaChannelWithCurrentBrush) const
        //

        g.drawImageAt(background, 0,0,false);
        g.drawImage(instrument1, 485,280,   95,95,   0,0,400,400,false);


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



    void playButtonClicked(void)
    {

    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));

        drawScene(g);


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
    }

    void resized() override
    {

        sceneConfigButton.setBounds(50, 50, MUTE_BUTTON_SIZE, MUTE_BUTTON_SIZE);
    }

private:

    Image background;
    Image instrument1;

    juce::ImageButton sceneConfigButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneComponent)

};//scene componenet.h


#endif //SCENE_COMPONENT_H