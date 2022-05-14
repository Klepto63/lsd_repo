#pragma once

#ifndef SCENE_CFG_H
#define SCENE_CFG_H

#include "JuceHeader.h"



typedef enum 
{
    E_SCENE_MODE_DEFAULT = 0,
    E_SCENE_MODE_A,
    E_SCENE_MODE_B,
    E_SCENE_MODE_FAR,
    E_SCENE_MODE_DUO,
    E_SCENE_MODE_ONE_CENTER, 
    E_SCENE_MODE_ONE_LEFT,
    E_SCENE_MODE_ONE_RIGHT,
    E_SCENE_MODE_ENUM,
}E_SCENE_MODE;

typedef enum 
{
    E_SCENE_AMBIANT_DEFAULT = 0,
    E_SCENE_AMBIANT_A,
    E_SCENE_AMBIANT_B,
    E_SCENE_AMBIANT_C,
    E_SCENE_AMBIANT_ENUM,
}E_SCENE_AMBIANT;

typedef enum 
{
    E_SCENE_LIVE_MODEOFF = 0,
    E_SCENE_LIVE_MODEON,
    E_SCENE_LIVE_MODE_ENUM,
}E_SCENE_LIVEMODE;


typedef struct
{
    E_SCENE_MODE mode;
    int mode_instrument;

    E_SCENE_AMBIANT ambiant;

    E_SCENE_LIVEMODE livemode;
    int livemodeInstrument;
}Scene_config;


void  sceneconfig_init(void);
void  sceneconfig_load(Scene_config* sceneconfif);
void  sceneconfig_save(Scene_config sceneconfif);
char* sceneconfig_text_mode(void);
char* sceneconfig_text_ambiant(void);
char* sceneconfig_text_live(void);


bool sceneconfig_pickinstr(E_SCENE_MODE mode);



char* sceneconfig_buttonText(int i, int j);

#endif SCENE_CFG_H