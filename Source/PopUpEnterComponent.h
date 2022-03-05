#pragma once
#ifndef POPUP_COMPONENT_H
#define POPUP_COMPONENT_H

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using namespace juce;

class PopUpEnterComponent  : public juce::Component
{
public:

    PopUpEnterComponent(int idx);
    ~PopUpEnterComponent() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void showPopupWindows(void);

private:

    int popupIdx = 1;

    TextButton ConnectPresquelButton;
    TextButton EnterWithoutPresquelButton;

    SafePointer<DialogWindow> dialogWindow;

    CustomLookAndFeel customLookAndFeel;


    void enterWithNoPresquel(void);
    void enterWithPresquel(void);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopUpEnterComponent)
};

#endif //POPUP_COMPONENT_H