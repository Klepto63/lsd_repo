#pragma once
#include <JuceHeader.h>

using namespace juce;


class EnergySliderLookAndFeel : public LookAndFeel_V1
{
public:

	EnergySliderLookAndFeel()
	{

	}

	~EnergySliderLookAndFeel()
	{

	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
	{
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        auto angleratio = (angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle);
        Colour c1 = Colour(ENERGY_SLIDER_RED_COLOR);
        Colour c2 = Colour(ENERGY_SLIDER_BLUE_COLOR);
        Colour c3 = c1.interpolatedWith(c2, angleratio).interpolatedWith(Colour(0xaaffffff), 1 - 2 * abs(angleratio - 0.5));

        //ellipse
        g.setColour(c3.interpolatedWith(Colour(WP_PLAYER), 0.7));
        g.drawEllipse(rx, ry, rw, rw, 1.0f);
    

        g.setColour(Colour(WP_PLAYER));
        g.drawLine(0, height, width, height,height);



        juce::Path p;
        auto pointerLength = radius * 0.30f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setColour(c3);
        g.fillPath(p);
	}


private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnergySliderLookAndFeel)
};




class OpenGlAngleSliderLookAndFeel : public LookAndFeel_V1
{


        public:

                OpenGlAngleSliderLookAndFeel()
                {

                }

                ~OpenGlAngleSliderLookAndFeel()
                {

                }

 
        private:

                JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGlAngleSliderLookAndFeel)
        
};