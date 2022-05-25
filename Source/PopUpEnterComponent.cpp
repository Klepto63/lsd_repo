

#include "PopUpEnterComponent.h"
#include "LeftBarComponent.h"
#include "Path.h"


void PopUpEnterComponent::constructPopUp(int idx)
{
	popupIdx = idx;
	BUTTON_A.setVisible(false);
	BUTTON_B.setVisible(false);
	BUTTON_C.setVisible(false);
	BUTTON_D.setVisible(false);
	BUTTON_A.onClick = [this] { ButtonA_callback(); };
	BUTTON_B.onClick = [this] { ButtonB_callback(); };
	BUTTON_C.onClick = [this] { ButtonC_callback(); };
	BUTTON_D.onClick = [this] { ButtonD_callback(); };

    switch (idx)
	{
	case POPUP_WELCOME:
	{
		BUTTON_C.setButtonText("OK ");
		BUTTON_C.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_C);
		setSize(600, 350);
		break;
	}
	case POPUP_CONNECT_PRESQUEL:
	{
		BUTTON_D.setButtonText("Cancel");
		BUTTON_D.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_D);
		BUTTON_B.setButtonText("Use mouse");
		BUTTON_B.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_B);
		BUTTON_C.setButtonText("Connect");
		BUTTON_C.setColour(0x1000100, Colour((uint32)BUTTON_COLOR2));
		addAndMakeVisible(BUTTON_C);

		

		//setSize(600, 350);	
		setSize(600, 500);	
		break;
	}
	case POPUP_LOOKING_FOR_PRESQUEL:
	{
		BUTTON_D.setButtonText("Cancel");
		BUTTON_D.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_D);

		BUTTON_B.setButtonText("Next"); //grisé
		BUTTON_B.setEnabled(false);

		BUTTON_B.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_B);
        //timer.startTimerHz(1);
		setSize(600, 650);

		break;
	}
	case POPUP_MOUSE_MODE:
	{	
		BUTTON_D.setButtonText("Back");
		BUTTON_D.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_D);

		
		BUTTON_B.setButtonText("Use mouse"); //grisé
		BUTTON_B.setColour(0x1000100, Colour((uint32)BUTTON_COLOR1));
		addAndMakeVisible(BUTTON_B);
		setSize(600, 500);
		break;
	}
	default:
	{
		//error
	}
	}
}



//==============================================================================
PopUpEnterComponent::PopUpEnterComponent(int idx)
{

    popupIdx = idx;
    setLookAndFeel(&customLookAndFeel);
    for (auto child : getChildren())
    {
        child->setLookAndFeel(&customLookAndFeel);
    }
	
	constructPopUp(idx);

    setOpaque(true);
}

PopUpEnterComponent::~PopUpEnterComponent()
{
    setLookAndFeel(nullptr);
    for (auto child : getChildren())
    {
        child->setLookAndFeel(nullptr);
    }
}


void PopUpEnterComponent::ButtonA_callback(void)
{
		switch(popupIdx)
	{
		case POPUP_WELCOME :
		{
			break;
		}
		case POPUP_CONNECT_PRESQUEL :
		{
			break;
		}
		case POPUP_LOOKING_FOR_PRESQUEL :
		{
			break;
		}
		case POPUP_MOUSE_MODE :
		{
			break;
		}
		default:
		{
			//error
		}
	}
}

void PopUpEnterComponent::ButtonB_callback(void)
{
	switch(popupIdx)
	{
		case POPUP_WELCOME :
		{
		}
			break;
		case POPUP_CONNECT_PRESQUEL :	//use mouse
		{
			constructPopUp(POPUP_MOUSE_MODE);
			resized();
			repaint();
			break;
		}
		case POPUP_LOOKING_FOR_PRESQUEL :
		{
			break;
		}
		case POPUP_MOUSE_MODE :
		{
			leftbarChangeCallback(LeftBarComponent::LB_FAKE_MODE);
			setVisible(false);
			break;
		}
		default:
		{
			//error
		}
	}
 }

void PopUpEnterComponent::ButtonC_callback(void)
{
	switch(popupIdx)
	{
		case POPUP_WELCOME :
		{
            setVisible(false);
			break;
		}
		case POPUP_CONNECT_PRESQUEL : //connect
		{	
			constructPopUp(POPUP_LOOKING_FOR_PRESQUEL);
			resized();
			repaint();
			break;
		}
		case POPUP_LOOKING_FOR_PRESQUEL :
		{
			break;
		}
		case POPUP_MOUSE_MODE :
		{
			break;
		}
		default:
		{
			//error
		}
	}
	return;
}

void PopUpEnterComponent::ButtonD_callback(void)
{
	switch(popupIdx)
	{
		case POPUP_WELCOME :
		{
			juce::URL url0("www.google.com");
			//delete this;
			break;
		}
		case POPUP_CONNECT_PRESQUEL :
		case POPUP_LOOKING_FOR_PRESQUEL :
		{
            setVisible(false);
			break;
		}
		case POPUP_MOUSE_MODE : //back to screen
		{
			constructPopUp(POPUP_CONNECT_PRESQUEL);
			repaint();
			resized();
			break;
		}
		default:
		{
			//error
		}
	}
	return;
}


//==============================================================================
void PopUpEnterComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colour((uint32)WP1_COLOR));
    String m;
    int h = getHeight();
    int w = getWidth();

    g.setFont(CODAFRONT_TEXT_SIZE_P1);
    static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
    Font Gofont = Font(typeface);
    Gofont.setHeight(34);

	switch(popupIdx)
	{
		case POPUP_WELCOME :
		{		
    		g.setFont(Gofont);
    		g.setColour(Colour((uint32)P1_COLOR));

    		g.drawText ("Welcome to Studio-Coda", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		m << "This is a demonstrator of our virtual 3D audio player. If you own a Presquel, then connect it now. If not, you can use mouse to simulate head moves." << newLine
        	<< "Thank you for your interest !";	
    		Gofont.setHeight(16);
    		g.setFont(Gofont);
    		g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);			
			break;
		}
		case POPUP_CONNECT_PRESQUEL :
		{
			int lbox = 180;
			displayedImage = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_METRONOM)));			
			g.drawImage(displayedImage,
						0.5 * (w - lbox),
						135,
						lbox,lbox,
						0,0,
            			displayedImage.getWidth(),
            			displayedImage.getHeight(),
            			false);

    		g.setFont(Gofont);
    		g.setColour(Colour((uint32)P1_COLOR));
    		g.drawText ("Connect", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		m << "Lets connect. Alternatively, use mouse";	
    		Gofont.setHeight(16);
    		g.setFont(Gofont);
    		g.drawFittedText(m,  Rectangle<int>(100,305, getWidth()-200, 150), juce::Justification::centred,10,1);
			break;
		}
		case POPUP_LOOKING_FOR_PRESQUEL :
		{
			int lbox = 230;
			displayedImage = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_SEARCHING_BLE)));			
			g.drawImage(displayedImage,
						0.5 * (w - lbox),
						205,
						lbox,lbox,
						0,0,
            			displayedImage.getWidth(),
            			displayedImage.getHeight(),
            			false);

    		g.setFont(Gofont);
    		g.setColour(Colour((uint32)P1_COLOR));
    		g.drawText ("Searching", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		 m << "searching..";
			Gofont.setHeight(16);
    		g.setFont(Gofont);
    		g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
			break;
		}
		case POPUP_MOUSE_MODE : 
		{
			int lbox = 150;
			displayedImage = ImageFileFormat::loadFrom(File::File(PathGetAsset(IMAGE_FAKEMOUSE)));	
			g.drawImage(displayedImage,
						0.5 * (w - lbox),
						150,
						lbox,lbox,
						0,0,
            			displayedImage.getWidth(),
            			displayedImage.getHeight(),
            			false);
    		g.setFont(Gofont);
    		g.setColour(Colour((uint32)P1_COLOR));
    		g.drawText ("Mouse mode", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);
    		m << "Use this button to simulate";	
    		Gofont.setHeight(16);
    		g.setFont(Gofont);
    		g.drawFittedText(m,  Rectangle<int>(100,305, getWidth()-200, 150), juce::Justification::centred,10,1);

			break;
		}



		default:
		{
			//error
		}
	}

}


void PopUpEnterComponent::showPopupWindows(void)
{
    String m;
    m << "Buy Presquel at www.studio-coda.com/order" << newLine
        << newLine
        << "Use the mouse to simulate head.";

    DialogWindow::LaunchOptions options;
    auto* label = new Label();
    label->setText(m, dontSendNotification);
    label->setColour(Label::textColourId, Colours::whitesmoke);
    options.content.setOwned(label);

    Rectangle<int> area(0, 0, 300, 200);
    options.content->setSize(area.getWidth(), area.getHeight());
    options.dialogTitle = "Thank you !";
    options.dialogBackgroundColour = Colour(0xff111111);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = false;
    options.resizable = true;

    dialogWindow = options.launchAsync();

    if (dialogWindow != nullptr)
        dialogWindow->centreWithSize(300, 200);
}

void PopUpEnterComponent::resized()
{
	int w = getWidth();
	int Y = 272;
	float space = 40;
	int sizeX = 110;
	int sizeY = 40;

	switch(popupIdx)
	{
		case POPUP_WELCOME :
		{		
			Y = 272;
			space = 40;
			sizeX = 110;
			sizeY = 40;
			BUTTON_C.setBounds((w - sizeX) * 0.5, Y, sizeX, sizeY);

			//default 
    		//float space = 60;
		    //Rectangle<int> buttonSize(0, 0, 170, 60);
    		//Rectangle<int> area((getWidth() / 2) - (buttonSize.getWidth()+0.5*space),(getHeight() / 1.12f) - buttonSize.getHeight(), 
    		//buttonSize.getWidth(), buttonSize.getHeight());
    		//BUTTON_C.setBounds(area.reduced(2));
			//BUTTON_D.setBounds(area.reduced(2));
    		//BUTTON_C.setBounds(area.translated(buttonSize.getWidth()+50,0).reduced(2) );
			
			break;
		}
		case POPUP_CONNECT_PRESQUEL : //D B C
		{
    		//float space = 40;
		    //Rectangle<int> buttonSize(0, 0, 110, 40); //0 0 170 60
    		//Rectangle<int> area((getWidth() / 2) - (buttonSize.getWidth()+0.5*space),
			//				   (getHeight() / 1.12f) - buttonSize.getHeight(), 
    		//					buttonSize.getWidth(), 
			//					buttonSize.getHeight());
    		//BUTTON_C.setBounds(area.reduced(2));

			Y = 435;
			space = 120;
			sizeX = 110;
			sizeY = 40;

			BUTTON_D.setBounds((w * 0.5 - space - sizeX), Y, sizeX, sizeY);
			BUTTON_B.setBounds((w - sizeX) * 0.5, Y, sizeX, sizeY);
			BUTTON_C.setBounds(w * 0.5 + space , Y, sizeX, sizeY);

			//BUTTON_D.setBounds(area.reduced(2));
			//BUTTON_B.setBounds(area.translated(buttonSize.getWidth()+25,0).reduced(2) );
    		//BUTTON_C.setBounds(area.translated(buttonSize.getWidth()+70,0).reduced(2) );

			break;
		}

		case POPUP_LOOKING_FOR_PRESQUEL :
		{
			Y = 272+300;
			space = 120;
			sizeX = 110;
			sizeY = 40;
			BUTTON_D.setBounds((w * 0.5 - space - sizeX), Y, sizeX, sizeY);
			BUTTON_B.setBounds(w * 0.5 + space , Y, sizeX, sizeY);
			break;
		}
		case POPUP_MOUSE_MODE: 
		{	
			Y = 435;
			space = 120;
			sizeX = 110;
			sizeY = 40;
			BUTTON_D.setBounds((w * 0.5 - space - sizeX), Y, sizeX, sizeY);
			BUTTON_B.setBounds(w * 0.5 + space , Y, sizeX, sizeY);	
			//BUTTON_B.setBounds((w - sizeX) * 0.5, Y, sizeX, sizeY);					
			break;
		}
		default:
		{
			//error
			break;
		}
	}

    //BUTTON_A.setBounds(area.translated(buttonSize.getWidth()+50,0).reduced(2) );
}
