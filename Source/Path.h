#ifndef PATH_H
#define PATH_H

#include "JuceHeader.h"

#define COMPUTER_PC  
     

#ifdef COMPUTER_PC
//const juce::String ABS_ROOT         = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/";
const juce::String ABS_PATH 		= "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/";
//const juce::String ABS_PATH_SONGS 	= 
const juce::String ABS_PATH_ASSETS 	= "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/";
const juce::String ABS_PATH_ASSETS_IMG 	= "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/";
#endif


typedef enum 
{
    PATH_SONG = 0,
    ASSET_PLAY_BUTTON,
    ASSET_PAUSE_BUTTON,
    ASSET_SOUND_MUTED,
    ASSET_SOUND_MUTED_ONCLICK,
    ASSET_SOUND1,
    ASSET_SOUND2,
    ASSET_SOUND3,

    ASSET_SOUND_ONCLICK,
    ASSET_SCENE_CONFIG,
    ASSET_3DBUTTON_OFF,
    ASSET_3DBUTTON_ON,
    ASSET_VST,
    ASSET_XML_DATA,
    ASSET_IMG_3D_BUTTON,
    ASSET_IMG_3D_BUTTON_ONCLICK,
    ASSET_IMG_NEXT_BUTTON,
    ASSET_IMG_NEXT_BUTTON_ONCLICK,
    ASSET_IMG_PREV_BUTTON,
    ASSET_IMG_PREV_BUTTON_ONCLICK,


    IMAGE_POLAR_PLAN,
    IMAGE_METRONOM,
    IMAGE_FAKEMOUSE,
    IMAGE_SEARCHING_BLE,
    IMAGE_TRACKER_LOCKED,
    IMAGE_3D_GRID,

}PATH_GET_ASSET;


juce::String PathGetAsset(PATH_GET_ASSET a);



#endif 
