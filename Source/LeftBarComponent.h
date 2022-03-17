#pragma once
#ifndef LEFTBAR_COMPONENT_H
#define LEFTBAR_COMPONENT_H

#include <JuceHeader.h>
#include "codaFront.h"
#include "PopUpEnterComponent.h"
#include "openGLscene.h"
#include "EnergySliderLookAndFeel.h"

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
			ConnectCodaButton.setVisible(true);
			break;
		}
		case LB_PREQUEL_MODE:
		{
			Slider1.setLookAndFeel(&openGlAngleSliderLookAndFeel);
        	Slider1.setSliderStyle(Slider::LinearHorizontal);
			Slider1.hideTextBox(true);
        	Slider1.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        	Slider1.setColour(0x1001300, Colour(THUMB_COLOR_OPENGL));      //thumbColor (la boule)
        	Slider1.setColour(0x1001310, Colour(LIGNE_COLOR_OPENGL));      //ligneColor
			Slider1.setValue(5, dontSendNotification);
        	Slider1.setRange(0, 10);
			

			Slider2.setLookAndFeel(&openGlAngleSliderLookAndFeel);
        	Slider2.setSliderStyle(Slider::LinearVertical);
			Slider2.hideTextBox(true);
        	Slider2.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        	Slider2.setColour(0x1001300, Colour(THUMB_COLOR_OPENGL));      //thumbColor (la boule)
        	Slider2.setColour(0x1001310, Colour(LIGNE_COLOR_OPENGL));      //ligneColor			
			Slider2.setValue(5, dontSendNotification);
        	Slider2.setRange(0, 10);

			addAndMakeVisible(&Slider1);
			addAndMakeVisible(&Slider2);


			addAndMakeVisible(&maskComponent);
			maskComponent.set_CodaIsConnected(false);

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
			int w = 160;
			int h = 160;
			int Y = 40;

			int h_slider = 15;

			maskComponent.setBounds((int)((getWidth() - w) * 0.5f), Y, w, h);
			w = w + 10; //rallongement bar
			int Y2 = Y + h + 0.7f*h_slider;
			Slider1.setBounds((int)((getWidth() - w) * 0.5f),Y2,w,h_slider);
			Slider2.setBounds( (int)((getWidth() - w) * 0.5f) + w+	h_slider, Y, h_slider, w);
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

	int state = 2;
    int windowInstance = 0; //nombre de pop ouvert simultanné
    Array<Component::SafePointer<Component>> windows;
	PopUpEnterComponent* dw = 0;

    TextButton ConnectCodaButton;
	openGLMaskComponent maskComponent;
	OpenGlAngleSliderLookAndFeel openGlAngleSliderLookAndFeel;
	Slider Slider1;
	Slider Slider2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftBarComponent)
};


#endif //LEFTBAR_COMPONENT_H