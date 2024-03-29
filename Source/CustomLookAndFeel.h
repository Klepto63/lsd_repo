#pragma once



#include <JuceHeader.h>

#include "codaFront.h"

using namespace juce;

class CustomLookAndFeel : public LookAndFeel_V1
{
public:
    CustomLookAndFeel()
    {
        setDefaultSansSerifTypeface(getCustomFont());
    }
    static const Typeface::Ptr getCustomFont()
    {
        static auto typeface = Typeface::createSystemTypefaceFor(CodaBinaryFont::GothamLight_ttf, CodaBinaryFont::GothamLight_ttfSize);
        return typeface;
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        auto edge = 2;

        buttonArea.removeFromLeft(edge);
        buttonArea.removeFromTop(edge);

        //// shadow
        g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
        g.fillRect(buttonArea);

        auto offset = isButtonDown ? -edge / 2 : -edge;
        buttonArea.translate(offset, offset);

        g.setColour(Colour((uint32)BUTTON_CUSTUM_COLOR));
        g.fillRect(buttonArea);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool, bool isButtonDown) override
    {
        auto font = getTextButtonFont(button, button.getHeight());
        g.setFont(font);
        //g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId : juce::TextButton::textColourOffId)
        //    .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        g.setColour(Colour((uint32)P1_COLOR)); 



        auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

        auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 2 : 1));
        auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 2 : 1));
        auto textWidth = button.getWidth() - leftIndent - rightIndent;

        auto edge = 2;
        auto offset = isButtonDown ? edge / 1 : 0;

        if (textWidth > 0)
            g.drawFittedText(button.getButtonText(),
                leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                juce::Justification::centred, 2);
    }





private:

    Typeface::Ptr getTypefaceForFont(const Font& f) override
    {
        return getCustomFont();
    }
};