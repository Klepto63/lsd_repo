#pragma once
#ifndef LEFTBAR_COMPONENT_H
#define LEFTBAR_COMPONENT_H

#include <JuceHeader.h>
#include "codaFront.h"
#include "PopUpEnterComponent.h"

using namespace juce;

//==============================================================================
class LeftBarComponent : public juce::Component
{
public:



    LeftBarComponent()
    {
        addAndMakeVisible(&ConnectCodaButton);
        ConnectCodaButton.onClick = [this] { ConnectCodaButtonClicked(); };
        ConnectCodaButton.setEnabled(true);
        ConnectCodaButton.setColour(0x1000100,Colour((uint32)BUTTON_COLOR1));
        ConnectCodaButton.setButtonText("Connect Prequel");
    }

    ~LeftBarComponent()
    {

    }

    void resized() override
    {
        int w = 160;
        int h = 22;
        ConnectCodaButton.setBounds((int)((getWidth()-w)*0.5f), (int)((getHeight()-h)*0.5f), w,h);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(Colour(WP_LEFTBAR));
    }

private:

    void ConnectCodaButtonClicked(void)
    {
        //for (auto& window : windows)
        //    window.deleteAndZero();
        //
        //windows.clear();

        if(!windowInstance)
        {
            String name = "Studio-Coda 1.0.0";
            Colour colour = Colour(WP2_COLOR);
            auto* dw = new DocumentWindow(name, colour,1, 1);
            //dw->setUsingNativeTitleBar (true);
            dw->setContentOwned(new PopUpEnterComponent(5), true);
           
            dw->setVisible(true);
            windows.add(dw);
            windowInstance++;
            //this->setVisible(false); supprime le component ici la left bar
        }
        //Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
        //showPopupWindows();
    }

    int windowInstance = 0; //nombre de pop ouvert simultanné
    Array<Component::SafePointer<Component>> windows;
    juce::TextButton ConnectCodaButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftBarComponent)
};


#endif //LEFTBAR_COMPONENT_H