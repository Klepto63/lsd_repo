#pragma once

#ifndef POLAR_PLAN_H
#define POLAR_PLAN_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"

#include "customLookAndFeel.h"


class PolarPlanComponent : public juce::AnimatedAppComponent
{
public:
    PolarPlanComponent()
    {
        setSize(100, 100);
        setFramesPerSecond(1);

        polarPlan = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_POLAR_PLAN)));
    }
    ~PolarPlanComponent()
    {

    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.


    }


  
    void drawCirclePolar(juce::Graphics& g, float dist, float angle, float radSize, juce::Colour color)
    {
        //int CenterX = 0;
        //int CenterY = 0;
        auto w = getLocalBounds().toFloat().getWidth();
        auto h = getLocalBounds().toFloat().getHeight();
        g.setColour(color);
        angle += 90;
        float x = 0.5 + dist * (std::cos(angle * 2 * 3.1415 / 360));
        float y = 0.88 - dist * (std::sin(angle * 2 * 3.1415 / 360));

        g.fillEllipse((w * x) - (radSize * 0.5f), (h * y) - (radSize * 0.5f), radSize, radSize);

        g.setColour(Colour(juce::Colours::black));
    	g.drawFittedText("piano",  Rectangle<int>(w*x - 30, y*h - 30, 60, 20), juce::Justification::centred,10,1);

    }


    void paint(juce::Graphics& g) override
    {
        int h = getHeight();
        int w = getWidth();
        g.fillAll(juce::Colours::green);
        g.drawImage(polarPlan,
            0, 0,    //dest
            w, h,
            0, 0,    //img
            polarPlan.getWidth(),
            polarPlan.getHeight(),
            false);

 
        drawCirclePolar(g, 0.2f, 0,  10 ,  juce::Colours::blue);  
        drawCirclePolar(g, 0.2f, 90, 10, juce::Colours::red);
        drawCirclePolar(g, 0.2f, -90, 10, juce::Colours::green);



    }



    void resized() override
    {

    }


private:
    Image polarPlan;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PolarPlanComponent)
};


#endif