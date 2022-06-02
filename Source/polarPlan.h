#pragma once

#ifndef POLAR_PLAN_H
#define POLAR_PLAN_H

#include <map>
#include <JuceHeader.h>
#include "Path.h"

#include "customLookAndFeel.h"
#include "sceneconfig.h"


#define SLOT_FRONT_DIST 0.35f
#define SLOT_FAR_DIST   0.82f

#define SLOT_MIDLE_DIST 0.45f
#define SLOT_MIDLE_LEFT_ANGLE   18.0f
#define SLOT_MIDLE_LLEFT_ANGLE  46.0f

#define SLOT_FRONT_LEFT_ANGLE  43.0f


class PolarPlanComponent : public juce::AnimatedAppComponent
{

typedef struct 
{
    String name;
    float dist;
    float   rad; 
}s_pp_instr;

public:

    PolarPlanComponent()
    {
        setSize(100, 100);
        setFramesPerSecond(1);
        polarPlan = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_POLAR_PLAN)));
        for(int ii = 0; ii < MAX_INSTR; ii++)
        {
            inst[ii].dist = -1; 
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

    void replot(Scene_config sc, int IDX)
    {

        for (int idx = 0; idx < jsonParserGetStemNumber(IDX); idx++)
        {
            e_slot slot = sc.slots[idx];
            inst[idx].name = jsonParserGetStemName(IDX, idx);
            switch (slot)
            {
            case E_SLOT_UNUSED:
            {
                inst[idx].dist = -1;
                break;
            }
            case E_SLOT_FRONT_CENTER:
            {
                inst[idx].rad = 0;
                inst[idx].dist = SLOT_FRONT_DIST;
                break;
            }
            case E_SLOT_FRONT_LEFT:
            {
                inst[idx].dist = SLOT_FRONT_DIST;
                inst[idx].rad = SLOT_FRONT_LEFT_ANGLE;
                break;
            }
            case E_SLOT_FRONT_RIGHT:
            {
                inst[idx].dist = SLOT_FRONT_DIST;
                inst[idx].rad = -1.0f * SLOT_FRONT_LEFT_ANGLE;
                break;
            }
            case E_SLOT_MIDLE_LEFT:
            {
                inst[idx].dist = SLOT_MIDLE_DIST;
                inst[idx].rad = 1.0f * SLOT_MIDLE_LEFT_ANGLE;
                break;
            }
            case E_SLOT_MIDLE_LLEFT:
            {
                inst[idx].dist = 1.0f * SLOT_MIDLE_DIST;
                inst[idx].rad = 1.0f * SLOT_MIDLE_LLEFT_ANGLE;
                break;
            }
            case E_SLOT_MIDLE_RIGHT:
            {
                inst[idx].dist = SLOT_MIDLE_DIST;
                inst[idx].rad = -1 * SLOT_MIDLE_LEFT_ANGLE;
                break;
            }
            case E_SLOT_MIDLE_RRIGHT:
            {
                inst[idx].dist = SLOT_MIDLE_DIST;
                inst[idx].rad = -1 * SLOT_MIDLE_LLEFT_ANGLE;
                break;
            }
            case E_SLOT_MIDLE_CENTER :
            {
                inst[idx].dist = SLOT_MIDLE_DIST;
                inst[idx].rad = 0;
                break;
            }
        }
            repaint();

        }
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

        for (int ii = 0; ii < MAX_INSTR; ii++)
        {
            if(inst[ii].dist != -1)
            {
                angle = inst[ii].rad + 90;
                x = 0.5 + inst[ii].dist * (std::cos((angle) * 2 * 3.1415 / 360));
                y = 0.88 - inst[ii].dist * (std::sin((angle) * 2 * 3.1415 / 360));
                g.setColour(Colour((uint32)POLAR_PLAN_LE_DOT));
                g.fillEllipse((w * x) - (10 * 0.5f), (h * y) - (10 * 0.5f), 11, 11);
                g.setFont(14);
                g.setColour(Colour(juce::Colours::white));
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