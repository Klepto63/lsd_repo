#pragma once
#ifndef LEFTBAR_COMPONENT_H
#define LEFTBAR_COMPONENT_H

#include <JuceHeader.h>
#include "codaFront.h"
#include "PopUpEnterComponent.h"
#include "openGLscene.h"
#include "EnergySliderLookAndFeel.h"
#include "customLookAndFeel.h"
#include "Path.h"

using namespace juce;



typedef std::function<void (float)> LeftbarCallback;

//==============================================================================
class LeftBarComponent : public juce::Component
{
public:

	typedef enum
	{
		LB_NO_CONNECTED = 1,
		LB_PREQUEL_MODE,
		LB_FAKE_MODE
	}LB_MODE;


	LeftBarComponent()
	{
        recordingTrackerLOCKED   = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_TRACKER_LOCKED)));
		socleconfigurationLOCKED = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_TRACKER_LOCKED)));
        setLookAndFeel(&customLookAndFeel);
        for (auto child : getChildren())
        {
            child->setLookAndFeel(&customLookAndFeel);
        }
		load_leftbar(state);
	}

	~LeftBarComponent()
	{

	}

	void resized() override
	{
		int width = getWidth();
		int height = getHeight(); 
		switch (state)
		{
		case LB_NO_CONNECTED:
		{
			int w = 160;
			int h = 30;
			ConnectCodaButton.setBounds((int)((width - w) * 0.5f), (int)((height - h) * 0.5f), w, h);
			break;
		}
		case LB_PREQUEL_MODE:
		{
			//int w = 160;
			//int h = 160;
			//int Y = 40;
			//int h_slider = 15;
			//maskComponent.setBounds((int)((width - w) * 0.5f), Y, w, h);
			//w = w + 10; //rallongement bar
			//int Y2 = Y + h + 0.7f*h_slider;
			//Slider1.setBounds((int)((width - w) * 0.5f),Y2,w,h_slider);
			//Slider2.setBounds( (int)((width- w) * 0.5f) + w+	h_slider, Y, h_slider, w);
			break;
		}
		case LB_FAKE_MODE:
		{


			int w = 60;
			int h = 60;
			int Y = 80;

			int h_slider = 15;

			maskComponent.setBounds((int)((width- w) * 0.5f), Y, w, h);


			w = w + 10; //rallongement bar
			int Y2 = Y + h + 0.7f*h_slider;
			Slider1.setBounds((int)((width- w) * 0.5f),Y2,w,h_slider);
			Slider2.setBounds( (int)((width - w) * 0.5f) + w+	h_slider, Y, h_slider, w);






			break;
		}
		default:
			break;
		}


	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(Colour(WP_LEFTBAR));

		if(state == LB_FAKE_MODE)
		{
			paint_connectionstate(g,0,0);
		}
	}

	void setletbarCallback(LeftbarCallback cb)
	{
		leftbarCallback = cb;
	}

	void setAngle(float f)
	{
		double a = (double) 1.5*180*f - 90;
		maskComponent.set_angle(0,a,0);
	}

private:


	void paint_connectionstate(juce::Graphics& g, bool coda, bool socle)
	{   
		int height = getHeight();
      	int width = getWidth();
		static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
        g.fillAll(Colour((uint32)WP_LEFTBAR));

        g.setColour(Colour((uint32)COLOR_TEXT_TABLE));
        Font Gofont = Font(typeface);
        Gofont.setHeight(16);		
		g.drawText ("Live mode :", Rectangle<int>(20, 0.37*height, getWidth(), 20), juce::Justification::left, false);	
		g.drawText ("Settings :", Rectangle<int>(20, 0.67*height, getWidth(), 20), juce::Justification::left, false);	
        g.setColour(Colour((uint32)TEXT_RED));		
		//g.drawText ("Headtracker : not connected", Rectangle<int>(20, height - 100, getWidth(), 150), juce::Justification::left, false);	

		int size = 0.15*height;
        g.drawImage(recordingTrackerLOCKED,

			0.5*(width - size), 0.40*height + 30,
			size,size,
            0, 0,    //img
            recordingTrackerLOCKED.getWidth(),
            recordingTrackerLOCKED.getHeight(),
            false);
        g.drawImage(recordingTrackerLOCKED,

			0.5*(width - size), 0.70*height + 30,
			size,size,
            0, 0,    //img
            recordingTrackerLOCKED.getWidth(),
            recordingTrackerLOCKED.getHeight(),
            false);

		Gofont.setHeight(18);	
		g.drawText ("No device detected", Rectangle<int>(0, 0.48*height + 30, getWidth(), 20), juce::Justification::horizontallyCentred, false);	
	}

	void load_leftbar(int vstate)
	{
		ConnectCodaButton.setVisible(false);
		Slider1.setVisible(false);
		Slider2.setVisible(false);
		state = vstate;

		switch (state)
		{
		case LB_NO_CONNECTED:
		{
			addAndMakeVisible(&ConnectCodaButton);
			ConnectCodaButton.onClick = [this] { ConnectCodaButtonClicked(); };
			ConnectCodaButton.setEnabled(true);
			ConnectCodaButton.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
			ConnectCodaButton.setButtonText("Connect device");
			ConnectCodaButton.setVisible(true);
			break;
		}
		case LB_PREQUEL_MODE:
		{
			//Slider1.setLookAndFeel(&openGlAngleSliderLookAndFeel);
        	//Slider1.setSliderStyle(Slider::LinearHorizontal);
			//Slider1.hideTextBox(true);
        	//Slider1.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        	//Slider1.setColour(0x1001300, Colour(THUMB_COLOR_OPENGL));      //thumbColor (la boule)
        	//Slider1.setColour(0x1001310, Colour(LIGNE_COLOR_OPENGL));      //ligneColor
			//Slider1.setValue(5, dontSendNotification);
        	//Slider1.setRange(0, 10);
			//Slider2.setLookAndFeel(&openGlAngleSliderLookAndFeel);
        	//Slider2.setSliderStyle(Slider::LinearVertical);
			//Slider2.hideTextBox(true);
        	//Slider2.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        	//Slider2.setColour(0x1001300, Colour(THUMB_COLOR_OPENGL));      //thumbColor (la boule)
        	//Slider2.setColour(0x1001310, Colour(LIGNE_COLOR_OPENGL));      //ligneColor			
			//Slider2.setValue(5, dontSendNotification);
        	//Slider2.setRange(0, 10);
			//addAndMakeVisible(&Slider1);
			//addAndMakeVisible(&Slider2);
			//addAndMakeVisible(&maskComponent);
			//maskComponent.set_CodaIsConnected(false);
			break;
		}
		case LB_FAKE_MODE:
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
			//addAndMakeVisible(&Slider1);
			//addAndMakeVisible(&Slider2);
			addAndMakeVisible(&maskComponent);
			maskComponent.set_CodaIsConnected(true);
			break;
		}
		default:
			break;
		}
		repaint();
		resized();
	}

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
			dw = new PopUpEnterComponent(POPUP_CONNECT_PRESQUEL);
			dw->setName("Connect device");
			dw->setVisible(true);
			dw->setOpaque(true);
			
			dw->setOnChangeCallback([this](int v)
			{	//child
				//do changes
				load_leftbar(v);
				leftbarCallback(v); //notify governor
			});

			dw->addToDesktop(ComponentPeer::windowHasTitleBar |
			ComponentPeer::windowAppearsOnTaskbar |
			ComponentPeer::windowHasMinimiseButton |
			ComponentPeer::windowHasCloseButton, 0); //StyleFlags
			dw->setAlwaysOnTop(true);
			dw->setCentreRelative(0.5, 0.4);
		}

            //windowInstance++;
            //this->setVisible(false); supprime le component ici la left bar
        //Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
        //showPopupWindows();
    }

	//int state = LB_FAKE_MODE;
	int state = LB_NO_CONNECTED;

    int windowInstance = 0; //nombre de pop ouvert simultanné
    Array<Component::SafePointer<Component>> windows;
	PopUpEnterComponent* dw = 0;

    TextButton ConnectCodaButton;
	openGLMaskComponent maskComponent;
	OpenGlAngleSliderLookAndFeel openGlAngleSliderLookAndFeel;
	CustomLookAndFeel customLookAndFeel;
	Slider Slider1;
	Slider Slider2;

    Image recordingTrackerLOCKED;
	Image socleconfigurationLOCKED;

	LeftbarCallback leftbarCallback; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftBarComponent)
};


#endif //LEFTBAR_COMPONENT_H