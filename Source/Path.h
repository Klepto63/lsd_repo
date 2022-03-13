#ifndef PATH_H
#define PATH_H

#define COMPUTER_PC  
//define COMPUTER_ZENBOOK         

#ifdef COMPUTER_ZENBOOK

const juce::String ABS_PATH 		= "C:/Users/alexa/Desktop/Coda2022/lsd_repo/Coda2022/";
const juce::String ABS_PATH_SONGS 	= "C:/Users/alexa/Desktop/Coda2022/songs/";
const juce::String ABS_PATH_ASSETS 	= "C:/Users/alexa/Desktop/Coda2022/lsd_repo/Coda2022/Assets/";
#endif

#ifdef COMPUTER_PC
const juce::String ABS_PATH 		= "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/";
const juce::String ABS_PATH_SONGS 	= "C:/Users/Alex/Desktop/Coda2022/songs/";
const juce::String ABS_PATH_ASSETS 	= "C:/Users/Alex/Desktop/Coda2022/lsd_repo/Coda2022/Assets/";
#endif

#endif 