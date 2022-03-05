#pragma once

#ifndef PATH_SONG_H
#define PATH_SONG_H

//todo faire un json file

#include "JuceHeader.h"
#include "Path.h"	

#define SONG_MAX_NUMBER 10

const juce::String PathSong_song1 = ABS_PATH_SONGS + (juce::String) "song1/song1.wav";
const juce::String PathSong_song2 = ABS_PATH_SONGS + (juce::String) "song2/song2.wav";
const juce::String PathImage_song1 = ABS_PATH_SONGS + (juce::String)"song1/song1.png";
const juce::String PathImage_song2 = ABS_PATH_SONGS + (juce::String)"song2/song2.png";


#endif