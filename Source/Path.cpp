#include "Path.h"


juce::String PathGetAsset(PATH_GET_ASSET a)
{
    juce::String ret;
    switch (a)
    {
    case PATH_SONG :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/songs/";
        break;
    }
    case ASSET_PLAY_BUTTON:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/playButton.png";
        break;
    }
    case ASSET_PAUSE_BUTTON:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/pauseButton.png";
        break;
    }
    case ASSET_SOUND_MUTED:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/soundMuted.png";
        break;
    }
    case ASSET_SOUND_MUTED_ONCLICK:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/soundMuted2.png";
        break;
    }
    case ASSET_SOUND1:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/sound1.png";
        break;
    }
    case ASSET_SOUND2:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/sound2.png";
        break;
    }
    case ASSET_SOUND3:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/sound3.png";
        break;
    }        
    case ASSET_SOUND_ONCLICK:
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/sound2.png";
        break;
    }
    case ASSET_SCENE_CONFIG : 
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/sceneConfig.png";        
        break;
    }
    case ASSET_3DBUTTON_OFF : 
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/3dbuttonOFF.png";        
        break;
    }
    case ASSET_3DBUTTON_ON : 
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/3dbuttonON.png";        
        break;
    }
    case ASSET_VST : 
    {   
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/dearVRpro.vst3";
        break;
    }
    case ASSET_XML_DATA : 
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/tabledata.xml";
        break;
    }
    case ASSET_IMG_3D_BUTTON :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/3dbutton.png";
        break;
    }
    case ASSET_IMG_3D_BUTTON_ONCLICK :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/3dbutton2.png";
        break;
    }
    case ASSET_IMG_NEXT_BUTTON :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/nextbutton.png";
        break;
    }
    case ASSET_IMG_NEXT_BUTTON_ONCLICK :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/nextbutton2.png";
        break;
    }
    case ASSET_IMG_PREV_BUTTON :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/prevbutton.png";        
        break;
    }
    case ASSET_IMG_PREV_BUTTON_ONCLICK :
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/prevbutton2.png";        
        break;
    }
    case IMAGE_POLAR_PLAN : 
    {
        ret = "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/polarplan.png";        
        break;
    }
    case IMAGE_METRONOM : 
    {
        ret =  "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/metronom.png";
        break;
    }
    case IMAGE_FAKEMOUSE : 
    {
        ret =  "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/fakemouse.png";
        break;
    }
    case IMAGE_TRACKER_LOCKED :
    {
        ret =  "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/locked.jpg";
        break;
    }
    case IMAGE_SEARCHING_BLE : 
    {
        ret =  "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/icons/searching.png";
        break;
    }
    case IMAGE_3D_GRID :
    {
        ret =  "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/images/gridd_.png";
        break;
    }
    default:
    {
    }
    }

    return ret;
}