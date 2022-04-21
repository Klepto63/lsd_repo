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

        polarPlan = ImageFileFormat::loadFrom(File::File(PathGetAsset(POLAR_PLAN)));
    }
    ~PolarPlanComponent()
    {

    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.


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
    }



    void resized() override
    {

    }


private:
    Image polarPlan;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PolarPlanComponent)
};


#endif