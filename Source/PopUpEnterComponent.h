#pragma once
#ifndef POPUP_COMPONENT_H
#define POPUP_COMPONENT_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using namespace juce;

typedef enum
{
    POPUP_WELCOME = 1,					//OK
    POPUP_CONNECT_PRESQUEL = 2,			//CANCEL  USE_FAKE	CONNECT
	POPUP_MOUSE_MODE = 3,
	POPUP_LOOKING_FOR_PRESQUEL = 4		// CANCEL		NEXT (blurred)
}ENUM_POPUP;


typedef std::function<void (float)> OnChangeCallback;

class PopUpEnterComponent  : public Component //todo timer
{
public:
    PopUpEnterComponent(int idx);
    ~PopUpEnterComponent() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void showPopupWindows(void);
	void setOnChangeCallback (OnChangeCallback cb) { leftbarChangeCallback = cb; }
    
private:

    int popupIdx = 1;
    TextButton BUTTON_A;
    TextButton BUTTON_B;
	TextButton BUTTON_C;
	TextButton BUTTON_D;

    Image displayedImage;


    void ButtonA_callback(void);
    void ButtonB_callback(void);
    void ButtonC_callback(void);
    void ButtonD_callback(void);
    void constructPopUp(int idx);


    CustomLookAndFeel customLookAndFeel;
    SafePointer<DialogWindow> dialogWindow;

    OnChangeCallback leftbarChangeCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopUpEnterComponent)
};

#endif //POPUP_COMPONENT_H*