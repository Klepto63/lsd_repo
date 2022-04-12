#pragma once
#ifndef WND_SCENE_CFG_H
#define WND_SCENE_CFG_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using namespace juce;


class WindowSceneConfig  : public Component //todo timer
{
public:

    WindowSceneConfig()
    {
        addAndMakeVisible(BUTTON_OK);
        BUTTON_OK.setVisible(false);
        BUTTON_OK.onClick = [this] {ButtonOK_callback();};

        setLookAndFeel(&customLookAndFeel);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(&customLookAndFeel);
        }
        setSize(800, 800);
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
    }

    void paint (juce::Graphics& g) override
    {
	    g.fillAll(Colour(WP_LEFTBAR));
    }
    void resized() override
    {
        BUTTON_OK.setBounds(0,0,80,40);
    }

    //void showPopupWindows(void);
	
private:

    TextButton BUTTON_OK;


    CustomLookAndFeel customLookAndFeel;
    SafePointer<DialogWindow> dialogWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowSceneConfig)
};

#endif //WND_SCENE_CFG_H