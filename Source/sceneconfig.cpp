#include "sceneconfig.h"


using namespace juce;


//Scene_config internalSceneConfig = {
//    .mode = E_SCENE_MODE_DEFAULT,
//    .mode_instrument = 0,
//    .ambiant = E_SCENE_AMBIANT_DEFAULT,
//    .livemode = E_SCENE_LIVE_MODEOFF,
//    .livemodeInstrument = 0
//};

Scene_config internalSceneConfig;

    char* SCENE_MODE_TEXT[E_SCENE_MODE_ENUM] = { "Default","A","B","FAR","DUO","ONE" };
char* SCENE_AMBIANT_TEXT[E_SCENE_MODE_ENUM] = {"Default","A","B","C" };
char* SCENE_LIVEMODE_TEXT[E_SCENE_MODE_ENUM] = {"OFF","ON"};


void sceneconfig_init(void)
{
    internalSceneConfig.mode = E_SCENE_MODE_DEFAULT;
    internalSceneConfig.mode_instrument = 0;
    internalSceneConfig.ambiant = E_SCENE_AMBIANT_B;
    internalSceneConfig.livemode = E_SCENE_LIVE_MODEOFF;
    internalSceneConfig.livemodeInstrument = 0; 
}

void  sceneconfig_load(Scene_config* sceneconfig)
{
    sceneconfig->mode = internalSceneConfig.mode;
    sceneconfig->mode_instrument =internalSceneConfig.mode_instrument;
    sceneconfig->ambiant = internalSceneConfig.ambiant;
    sceneconfig->livemode = internalSceneConfig.livemode;
    sceneconfig->livemodeInstrument = internalSceneConfig.livemodeInstrument;
}
void  sceneconfig_save(Scene_config sceneconfif)
{
    internalSceneConfig.mode =               sceneconfif.mode;
    internalSceneConfig.mode_instrument =    sceneconfif.mode_instrument;
    internalSceneConfig.ambiant =            sceneconfif.ambiant;
    internalSceneConfig.livemode =           sceneconfif.livemode;
    internalSceneConfig.livemodeInstrument = sceneconfif.livemodeInstrument;
}

char* sceneconfig_text_mode(void)
{
    char* m = "hello text";
    return m;
}

char* sceneconfig_text_ambiant(void)
{
    char* m = "hello text";
    return m;
}

char* sceneconfig_text_live(void)
{
    char* m = "hello text";
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