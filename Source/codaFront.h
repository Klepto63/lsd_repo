#ifndef CODA_FONT_H
#define CODA_FONT_H


#define CODAFRONT_TEXT_SIZE_P1		(juce::Font (36.0f))
#define CODAFRONT_TEXT_COLOR		(juce::Colours::white)


//------------------------------------------

//buttonColourId = 0x1000100
//buttonOnColourId = 0x1000101, 
//textColourOffId = 0x1000102
//textColourOnId = 0x1000103

//#40bb91 ver style  https://www.peko-step.com/fr/tool/tfcolor.html
//rouge style 0xffff3054
//------------------------------------ POPUP1
#define WP1_COLOR	  0xff17171A                      //0xff111111
#define WP_POPUP      0xff000000

#define P1_COLOR      0xffffffff        //couleur text boutton custom
#define BUTTON_CUSTUM_COLOR 0xff121212  //couleur boutton custom
#define BUTTON_COLOR1 0xff181818
#define BUTTON_COLOR2 0xff116c31

//-------------------------------------SCENE COMPONENT
#define SCENE_COMPONENT_WP      WP1_COLOR
#define SCENE_COMPONENT_WP2     0xffff0000
#define SCENE_COMPONENT_LABEL   0xff9999AA

#define POLAR_PLAN_LE_DOT       0xff00AAAA      

//--------------------------------------- SLIDER MUSIC
#define BACKGROUND_COLOR     0xff565659
#define THUMB_COLOR          0x00000000                   //0xc8ffffff
#define LIGNE_COLOR          0xff00ffff       //rouge 0xffff3054//0xff1db954    //OLD VERT
#define VOLUME_LIGNE_COLOR   0xffEEEEEE



//--------------------------------------- TABLE
#define SELECTED_ROW_COLOR   0xAA555555    // remetttre0x991db954                   //0x10ffffff
//#define SELECTED_ROW_COLOR   0xffff3054  //remetttre 0x991db954                   //0x10ffffff
#define ROW_ALTERNATE_COLOR  0xff181818
#define ROW_ALTERNATE_COLOR2 0xff121212
#define COLOR_TEXT_TABLE     0xffcccccc

//--------------------------------------- PLAYBAR
#define WP_PLAYER       0xff202022     //0xff222226 TIDAL            //   0xff181818
#define PLAY_BUTTON_SIZE  35

#define NEXT_BUTTON_SIZE_X  18
#define NEXT_BUTTON_SIZE_Y  20
#define NEXT_BUTTON_SPACE 55
#define ICON_BUTTON_SIZE  24

#define ENERGY_SLIDER_RED_COLOR      0xaaffffff
//#define ENERGY_SLIDER_BLUE_COLOR     0xffeeeeee

//#define WP_PLAYER   0xff0000ff

//--------------------- LEFTBAR
#define WP_LEFTBAR 0xFF000000//0xff242528                  // 0xFF000000

#define TEXT_RED   0xA0FF5544 

#define TABLE_X0 500
#define TABLE_Y0 150
#define TABLE_X1 650
#define TABLE_Y1 500

//--------------------------------------- SLIDER OPENGL
#define THUMB_COLOR_OPENGL 0xc8ffffff
#define LIGNE_COLOR_OPENGL 0x550099aa

#define RECENTER_BUTTON 0xFF111111

namespace CodaBinaryFont
{
    extern const char*   GothamLight_ttf;
    const int            GothamLight_ttfSize = 56508;
}


#endif
