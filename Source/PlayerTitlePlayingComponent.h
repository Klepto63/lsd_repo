#pragma once

#include "JuceHeader.h"
#include "pathsong.h"
#include "JsonParser.h"

using namespace juce;

class PlayerTitlePlayingComponent : public juce::Component
{
public:
	PlayerTitlePlayingComponent()
	{
		//PathImage_song1
		//
		firstInit = false;
	}

	~PlayerTitlePlayingComponent()
	{

	}

	void loadSongData(s_metadata md)
	{
		title = md.title;
		composer = md.composer;
		image = ImageCache::getFromFile((String)md.pathImage);
		firstInit = true;
		repaint();
	}

	void paint(juce::Graphics& g) override
	{
		if (firstInit)
		{
			g.setColour(Colour(WP_PLAYER));

			g.drawRect(80, 0, 90,50,25);
			//g.fillAll(Colours::red);

			g.drawImage(image, 0, 0, 60, 60, 0, 0, image.getWidth(), image.getHeight(), false);

			g.setColour(Colour(0xffffffff));
			g.setFont(15);
			g.drawText(title, 82, 0, 150, 30, Justification::centredLeft);

			g.setColour(Colour(0xffbbbbbb));
			g.setFont(13);
			g.drawText(composer, 82, 15, 150, 40, Justification::centredLeft);

		}
	}

private:
		
	Image   image;
	String  title;
	String  composer;
	bool	firstInit;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerTitlePlayingComponent)
};