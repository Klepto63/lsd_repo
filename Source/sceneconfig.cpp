#include "sceneconfig.h"

using namespace juce;

#define MAX_SONG_NUMBER 20 //todo

Scene_config internalSceneConfig[MAX_SONG_NUMBER];

char* SCENE_MODE_TEXT[E_SCENE_MODE_ENUM] = {"A","B","C","FAR","ONE (Center)","ONE (Left)", "ONE (Right)"};
char* SCENE_AMBIANT_TEXT[E_SCENE_MODE_ENUM] = {"Default","A","B","C" };
char* SCENE_LIVEMODE_TEXT[E_SCENE_MODE_ENUM] = {"OFF","ON"};


typedef struct 
{
    int   idx;
    float dist;
    int   rad; 
    bool  active;
}s_sc_slot;

int hightLight[MAX_INSTR];

void sceneconfig_init(void)
{
    for(int ii = 0; ii < MAX_SONG_NUMBER; ii++)
    {
        internalSceneConfig[ii].mode = E_SCENE_MODE_A;
        internalSceneConfig[ii].mode_instrument = 0;
        internalSceneConfig[ii].ambiant = E_SCENE_AMBIANT_B;
        internalSceneConfig[ii].livemode = E_SCENE_LIVE_MODEOFF;
        internalSceneConfig[ii].livemodeInstrument = 0; 
        internalSceneConfig[ii].slots[0] = E_SLOT_MIDLE_LLEFT;
        internalSceneConfig[ii].slots[1] = E_SLOT_MIDLE_LEFT;
        internalSceneConfig[ii].slots[2] = E_SLOT_MIDLE_RIGHT;
        internalSceneConfig[ii].slots[3] = E_SLOT_MIDLE_RRIGHT;        
    }
}

void  sceneconfig_load(Scene_config* sceneconfig, int ii)
{
    sceneconfig->mode = (E_SCENE_MODE) internalSceneConfig[ii].mode;
    sceneconfig->mode_instrument =internalSceneConfig[ii].mode_instrument;
    sceneconfig->ambiant = internalSceneConfig[ii].ambiant;
    sceneconfig->livemode = internalSceneConfig[ii].livemode;
    sceneconfig->livemodeInstrument = internalSceneConfig[ii].livemodeInstrument;
    for(int i = 0; i < MAX_INSTR; i++)
    {
        sceneconfig->slots[i] = internalSceneConfig[ii].slots[i];
    }
}

void  sceneconfig_save(Scene_config sceneconfig, int ii)
{
    internalSceneConfig[ii].mode =               sceneconfig.mode;
    internalSceneConfig[ii].mode_instrument =    sceneconfig.mode_instrument;
    internalSceneConfig[ii].ambiant =            sceneconfig.ambiant;
    internalSceneConfig[ii].livemode =           sceneconfig.livemode;
    internalSceneConfig[ii].livemodeInstrument = sceneconfig.livemodeInstrument;
    for(int i = 0; i < MAX_INSTR; i++)
    {
        internalSceneConfig[ii].slots[i] = sceneconfig.slots[i];
    }
}

char* sceneconfig_text_mode(int ii)
{
    char* m;
    switch(internalSceneConfig[ii].mode)
    {
    case E_SCENE_MODE_A : 
    {
        m = "text mode A";
        break;
    }
    case E_SCENE_MODE_B : 
    {
        m = "text mode B";
        break;
    }
    case E_SCENE_MODE_C : 
    {
        m = "text mode duo";
        break;
    }    
    case E_SCENE_MODE_FAR : 
    {
        m = "text mode FAR";        
        break;
    }
    case E_SCENE_MODE_ONE_CENTER : 
    {
        m = "text mode center";
        break;
    }
    case E_SCENE_MODE_ONE_LEFT : 
    {
        m = "text mode left";
        break;
    }
    case E_SCENE_MODE_ONE_RIGHT : 
    {
        m = "text mode right";
        break;
    }
    default : 
    {
        m = "unkown";
    }
    }
    return m;
}

bool sceneconfig_should_pickinstr(E_SCENE_MODE mode)
{
    bool ret = false;

    if( mode == E_SCENE_MODE_ONE_CENTER ||
        mode == E_SCENE_MODE_ONE_LEFT ||
        mode == E_SCENE_MODE_ONE_RIGHT)
     {
         ret = true;
     }
    return ret;
}

char* sceneconfig_text_ambiant(int ii)
{
    char* m;
    switch(internalSceneConfig[ii].ambiant)
    {
    case E_SCENE_AMBIANT_DEFAULT:  
    {
         m = "ambiant default ambiant default ambiant default ambiant default ambiant default ambiant default ambiant defaultambiant defaultambiant default ";
        break;
    }
    case E_SCENE_AMBIANT_A :
    {
         m = "ambiant A ambiant A ambiant Aambiant Aambiant A ambiant A ambiant A ambiant Aambiant Aambiant A ambiant A ambiant A ambiant Aambiant Aambiant A";
        break;
    }
    case E_SCENE_AMBIANT_B :
    {
         m = "ambiant B ambiant B ambiant Bambiant Bambiant B ambiant Bambiant B ambiant B ambiant Bambiant B ambiant B ambiant Bambiant B ambiant B ambiant Bambiant B  ";
        break;
    }
    case E_SCENE_AMBIANT_C :
    {
         m = "ambiant C";
        break;
    }
    default : 
    {
         m = "unkown";
    }
    }
    return m;
}

char* sceneconfig_text_live(int ii)
{
     char* m;
    switch(internalSceneConfig[ii].livemode)
    {
        case E_SCENE_LIVE_MODEOFF : 
        {
            m = "live off live off live off live off live off live off  live off live off live off  live off live off live off live off live off live off  ";
            break;
        }
    
        case E_SCENE_LIVE_MODEON : 
        {
            m = "live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on live on";
            break;
        }
        default : 
        {
            m = "unkown";
        }
    }
    return m;
}

char* sceneconfig_buttonText(int i, int j)
{
    if(i == 0)
    {
        return(SCENE_MODE_TEXT[j]);
    }
    else if(i ==1)
    {
        return(SCENE_AMBIANT_TEXT[j]);
    }
    else if(i == 2)
    {
        return(SCENE_LIVEMODE_TEXT[j]);
    }
}

void sceneconfig_clearInstr(int IDX)
{

}

void sceneconfig_save_highlight(int* array)
{
    for(int ii = 0 ; ii < MAX_INSTR ; ii++)
    {
        hightLight[ii] = array[ii];
    }
}

void sceneconfig_reset_hightlight(void)
{
    for(int ii = 0 ; ii < MAX_INSTR ; ii++)
    {
        hightLight[ii] = 1;
    }  
}

float sceneconfig_get_hightlight(int i)
{
    bool isHilightMode = false;
    for(int ii = 0 ; ii < MAX_INSTR ; ii++)
    {
        if(hightLight[ii] == 2)
        {
            isHilightMode = true;
        }
    }

    if(isHilightMode)
    {
        if(hightLight[i] == -1)
        {
            return -5;
        }
        if(hightLight[i] == 1)
        {
            return 0.70f;
        }    
        if(hightLight[i] == 2)
        {
            return 0.80f;
        } 
    }
    else
    {
        return 0.8; //=0dB
    }

   
}