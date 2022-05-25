#pragma once

#ifndef POLAR_PLAN_H
#define POLAR_PLAN_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"

#include "customLookAndFeel.h"
#include "sceneconfig.h"


#define MAX_INSTR 5

class PolarPlanComponent : public juce::AnimatedAppComponent
{

typedef struct 
{
    String name;
    float dist;
    int   rad; 
    bool  active;
}s_pp_instr;

public:


    typedef enum
    {
        E_SLOT_FRONT_CENTER,
        E_SLOT_FRONT_LEFT,
        E_SLOT_FRONT_RIGHT,

    }e_slots;


    PolarPlanComponent()
    {
        setSize(100, 100);
        setFramesPerSecond(1);
        polarPlan = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_POLAR_PLAN)));
        for(int ii = 0; ii < MAX_INSTR; ii++)
        {
            inst[ii].active = false; 
        }

    }
    ~PolarPlanComponent()
    {
    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
    }


    void clearInstr(void)
    {
        for(int ii = 0; ii <MAX_INSTR; ii++)
        {
            inst[ii].active = false; 
        }
        repaint();
    }

    void addInstr(e_slots slot, String instrName)
    {
        int idx=0;
        while(inst[idx].active && idx < MAX_INSTR)
        {
            idx++;
        }
        if(idx == MAX_INSTR)
        {
            return;
        }

        inst[idx].name = instrName;
        inst[idx].active = true;
        inst[idx].dist = 0.2f;        
        switch(slot)
        {
            case E_SLOT_FRONT_CENTER :
            {
                inst[idx].rad = 0;
                break;
            }
            case E_SLOT_FRONT_LEFT :
            {
                inst[idx].rad = 42;
                break;
            }
            case E_SLOT_FRONT_RIGHT :
            {
                inst[idx].rad = -42;
                break;
            }                       
        }
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        int h = getHeight();
        int w = getWidth();
        float x,y = 0;
        int angle;
        g.fillAll(juce::Colours::green);
        g.drawImage(polarPlan,
            0, 0,    //dest
            w, h,
            0, 0,    //img
            polarPlan.getWidth(),
            polarPlan.getHeight(),
            false);

        for(int ii = 0; ii<MAX_INSTR; ii++)
        {
            if(inst[ii].active)
            {
                angle = inst[ii].rad + 90;
                x = 0.5 + inst[ii].dist * (std::cos((angle) * 2 * 3.1415 / 360));
                y = 0.88 - inst[ii].dist * (std::sin((angle) * 2 * 3.1415 / 360));
                g.setColour(juce::Colours::red);
                g.fillEllipse((w * x) - (10 * 0.5f), (h * y) - (10 * 0.5f), 10, 10);
                g.setColour(Colour(juce::Colours::black));
    	        g.drawFittedText(inst[ii].name,  Rectangle<int>(w*x - 30, y*h - 30, 60, 20), juce::Justification::centred,10,1);                
            }
        }
    }

    void resized() override
    {

    }


private:



    Image polarPlan;
    s_pp_instr inst[MAX_INSTR];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PolarPlanComponent)
};


#endif