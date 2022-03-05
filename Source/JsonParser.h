#pragma once

#include "JuceHeader.h"
using namespace juce;

#define STEM_MAX_NB 5

typedef struct
{
	String name;
	String path;
}s_stem;

typedef struct
{
	String id;
	String title;
	String composer;
	String pathImage;
	int stem_nb;
	String pathSong;
	String folder;
	s_stem stem[STEM_MAX_NB];
}s_metadata;

void jsonParserInit(void);
int  jsonParserLoad(int idx, s_metadata* metadata);
int  jsonParserGetNbSong(void);