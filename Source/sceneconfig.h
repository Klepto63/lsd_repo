#pragma once

#ifndef SCENE_CFG_H
#define SCENE_CFG_H

#include "JuceHeader.h"

#define MAX_INSTR 5 //todo 5

typedef enum
{
    E_SLOT_UNUSED = 0,
    E_SLOT_FRONT_CENTER,
    E_SLOT_FRONT_LEFT,
    E_SLOT_FRONT_RIGHT,
    E_SLOT_MIDLE_LLEFT,
    E_SLOT_MIDLE_LEFT,
    E_SLOT_MIDLE_RIGHT,
    E_SLOT_MIDLE_RRIGHT,
    E_SLOT_MIDLE_CENTER
}e_slot;

typedef enum 
{
    E_SCENE_MODE_A = 0,
    E_SCENE_MODE_B,
    E_SCENE_MODE_C,
    E_SCENE_MODE_FAR,
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
    e_slot slots[MAX_INSTR];
    E_SCENE_AMBIANT ambiant;

    E_SCENE_LIVEMODE livemode;
    int livemodeInstrument;
}Scene_config;




void  sceneconfig_init(void);
void  sceneconfig_load(Scene_config* sceneconfif, int ii);
void  sceneconfig_save(Scene_config sceneconfif, int ii);
char* sceneconfig_text_mode(int ii);
char* sceneconfig_text_ambiant(int ii);
char* sceneconfig_text_live(int ii);

void  sceneconfig_save_highlight(int* array);
void  sceneconfig_reset_hightlight(void);
float sceneconfig_get_hightlight(int i);

bool  sceneconfig_should_pickinstr(E_SCENE_MODE mode);
char* sceneconfig_buttonText(int i, int j);

#endif SCENE_CFG_H