#pragma once
#ifndef LEFTBAR_COMPONENT_H
#define LEFTBAR_COMPONENT_H

#include <JuceHeader.h>
#include "codaFront.h"
#include "PopUpEnterComponent.h"
#include "openGLscene.h"

using namespace juce;


typedef enum
{
	LB_NO_CONNECTED = 1,
	LB_PREQUEL_MODE,
	LB_FAKE_MODE
}LB_MODE;

//==============================================================================
class LeftBarComponent : public juce::Component
{
public:

	LeftBarComponent()
	{
		switch (state)
		{
		case LB_NO_CONNECTED:
		{
			addAndMakeVisible(&ConnectCodaButton);
			ConnectCodaButton.onClick = [this] { ConnectCodaButtonClicked(); };
			ConnectCodaButton.setEnabled(true);
			ConnectCodaButton.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
			ConnectCodaButton.setButtonText("Connect");
			break;
		}
		case LB_PREQUEL_MODE:
		{
			break;
		}
		case LB_FAKE_MODE:
		{
			break;
		}
		default:
			break;
		}
	}

	~LeftBarComponent()
	{

	}

	void resized() override
	{
		switch (state)
		{
		case LB_NO_CONNECTED:
		{
			int w = 160;
			int h = 22;
			ConnectCodaButton.setBounds((int)((getWidth() - w) * 0.5f), (int)((getHeight() - h) * 0.5f), w, h);
			break;
		}
		case LB_PREQUEL_MODE:
		{
			break;
		}
		case LB_FAKE_MODE:
		{
			break;
		}
		default:
			break;
		}





	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(Colour(WP_LEFTBAR));
	}

	void setState(int i)
	{
		this->state = i;
	}

	int getState()
	{
		return this->state;
	}

private:

	void ConnectCodaButtonClicked(void)
	{
		//for (auto& window : windows)
		//    window.deleteAndZero();
		//
		//windows.clear();

		bool shouldPopup = false;
		if(dw == 0)
		{
			shouldPopup = true;
		}
		else if (dw->isVisible() == false)
		{
			shouldPopup = true;
		}
		if(shouldPopup)
		{
			String name = "Studio-Coda 1.0.0";
			Colour colour = Colour(WP2_COLOR);
			dw = new PopUpEnterComponent(POPUP_CONNECT_PRESQUEL);
			dw->setVisible(true);
			dw->setOpaque(true);
			dw->addToDesktop(ComponentPeer::windowHasTitleBar |
			ComponentPeer::windowAppearsOnTaskbar |
			ComponentPeer::windowHasMinimiseButton |
			ComponentPeer::windowHasCloseButton, 0); //StyleFlags
			dw->setAlwaysOnTop(true);
			dw->setCentreRelative(0.5, 0.5);
		}

            //windowInstance++;
            //this->setVisible(false); supprime le component ici la left bar
        //Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
        //showPopupWindows();
    }

	int state = 1;
    int windowInstance = 0; //nombre de pop ouvert simultanné
    Array<Component::SafePointer<Component>> windows;
	PopUpEnterComponent* dw = 0;
    juce::TextButton ConnectCodaButton;

	openGLMaskComponent maskComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftBarComponent)
};


#endif //LEFTBAR_COMPONENT_H