#ifndef CODA_FONT_H
#define CODA_FONT_H


#define CODAFRONT_TEXT_SIZE_P1		(juce::Font (36.0f))
#define CODAFRONT_TEXT_COLOR		(juce::Colours::white)


//------------------------------------------

//buttonColourId = 0x1000100
//buttonOnColourId = 0x1000101, 
//textColourOffId = 0x1000102
//textColourOnId = 0x1000103



//------------------------------------ POPUP1
#define WP1_COLOR	  0xff111111
#define P1_COLOR      0xffffffff
#define BUTTON_COLOR1 0xff181818
#define BUTTON_COLOR2 0xff116c31

//------------------------------------- GOUVERNOR
#define WP2_COLOR     0xff111111

//--------------------------------------- SLIDER MUSIC
#define BACKGROUND_COLOR 0xff535353
#define THUMB_COLOR      0xc8ffffff
#define LIGNE_COLOR      0xff1db954


//--------------------------------------- TABLE
#define SELECTED_ROW_COLOR   0x991db954                   //0x10ffffff
#define ROW_ALTERNATE_COLOR  0xff181818
#define ROW_ALTERNATE_COLOR2 0xff121212
#define COLOR_TEXT_TABLE     0xffbbbbbb

//--------------------------------------- PLAYBAR
#define WP_PLAYER 0xff181818
#define PLAY_BUTTON_SIZE  37
#define NEXT_BUTTON_SIZE  20
#define NEXT_BUTTON_SPACE 90
#define MUSIC_SLIDER_SIZE 400
#define MUTE_BUTTON_SIZE  19

#define ENERGY_SLIDER_RED_COLOR      0xaa39eeea
#define ENERGY_SLIDER_BLUE_COLOR     0xaa1db954

//#define WP_PLAYER   0xff0000ff

//--------------------- LEFTBAR
#define WP_LEFTBAR 0xFF000000

#define TABLE_X0 500
#define TABLE_Y0 150
#define TABLE_X1 650
#define TABLE_Y1 500

//--------------------------------------- SLIDER OPENGL
#define THUMB_COLOR_OPENGL 0xc8ffffff
#define LIGNE_COLOR_OPENGL 0x550099aa

namespace CodaBinaryFont
{
    extern const char*   GothamLight_ttf;
    const int            GothamLight_ttfSize = 56508;
}


#endif
