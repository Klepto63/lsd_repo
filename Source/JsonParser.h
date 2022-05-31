#pragma once

#include "JuceHeader.h"
#include "sceneconfig.h"
using namespace juce;

#define STEM_MAX_NB MAX_INSTR

typedef struct
{
	String name;
	String path;
	String img;
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

int    jsonParserGetStemNumber(int idxsong);
String jsonParserGetStemName(int idxsong, int id);
String jsonParserGetStemImgPath(int idxsong, int id);