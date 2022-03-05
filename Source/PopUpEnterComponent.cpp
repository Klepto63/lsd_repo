

#include "PopUpEnterComponent.h"

//==============================================================================
PopUpEnterComponent::PopUpEnterComponent(int idx)
{
    popupIdx = idx;
    setLookAndFeel(&customLookAndFeel);
    for (auto child : getChildren())
    {
        child->setLookAndFeel(&customLookAndFeel);
    }

    setOpaque(true);

    ConnectPresquelButton.onClick = [this] { enterWithNoPresquel(); };
    ConnectPresquelButton.setButtonText("no Presquel..");
    ConnectPresquelButton.setColour(0x1000100,Colour((uint32)BUTTON_COLOR1));

    addAndMakeVisible(ConnectPresquelButton);
    addAndMakeVisible(EnterWithoutPresquelButton);
    EnterWithoutPresquelButton.onClick = [this] { enterWithPresquel(); };
    EnterWithoutPresquelButton.setColour(0x1000100, Colour((uint32)BUTTON_COLOR2));


    char v = '5';

    if(popupIdx == 1)
    {
        v = '1';
    }

    EnterWithoutPresquelButton.setButtonText (&v);


    setSize(600, 350);
}

PopUpEnterComponent::~PopUpEnterComponent()
{
    setLookAndFeel(nullptr);
    for (auto child : getChildren())
    {
        child->setLookAndFeel(nullptr);
    }
}

void PopUpEnterComponent::enterWithNoPresquel(void)
{
    
    //for (auto& window : windows)
    //    window.deleteAndZero();
    //windows.clear();
    

    /*
    String name = "Studio-Coda 1.0.0";
    Colour colour = Colour(WP2_COLOR);
    auto* dw = new DocumentWindow(name, colour,1, 1);
    dw->setContentOwned(new MainContentComponent(), true);
    dw->setVisible(true);
    windows.add(dw);
    this->setVisible(false);
  
    Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
    showPopupWindows();
    */
 }

void PopUpEnterComponent::enterWithPresquel(void)
{

}

//==============================================================================
void PopUpEnterComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colour((uint32)WP1_COLOR));

    g.setFont(CODAFRONT_TEXT_SIZE_P1);
    static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
    Font Gofont = Font(typeface);
    Gofont.setHeight(34);
    g.setFont(Gofont);

    g.setColour(Colour((uint32)P1_COLOR));

    //Rectangle<int> textRectangle(0, 0, getWidth(), 150);
    g.drawText ("Welcome to Studio-Coda", Rectangle<int>(0, 0, getWidth(), 150), juce::Justification::centred, true);

    Gofont.setHeight(16);
    g.setFont(Gofont);

    String m;
    m << "This is a demonstrator of our virtual 3D audio player. If you own a Presquel, then connect it now. If not, you can use mouse to simulate head moves." << newLine
        << "Thank you for your interest !";
    g.drawFittedText(m,  Rectangle<int>(100, 75, getWidth()-200, 150), juce::Justification::centred,10,1);
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
    Rectangle<int> buttonSize(0, 0, 170, 60);
    float space = 60;
    Rectangle<int> area((getWidth() / 2) - (buttonSize.getWidth()+0.5*space),(getHeight() / 1.12f) - buttonSize.getHeight(), 
    buttonSize.getWidth(), buttonSize.getHeight());
    ConnectPresquelButton.setBounds(area.reduced(2));
    EnterWithoutPresquelButton.setBounds(area.translated(buttonSize.getWidth()+50,0).reduced(2) );
}
