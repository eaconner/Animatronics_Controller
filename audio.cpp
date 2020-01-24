/**
*   @file   audio.cpp
*   @brief  Functions for playing audio with the PT8211
*/

#include "audio.h"
#include "show.h"
#include <Audio.h>
#include <SD.h>

AudioPlaySdWav       sdWav;
AudioAmplifier       ampLeft;
AudioAmplifier       ampRight;
AudioOutputPT8211    pt8211;
AudioConnection      patchCord1(sdWav, 0, ampLeft, 0);
AudioConnection      patchCord2(sdWav, 1, ampRight, 0);
AudioConnection      patchCord3(ampLeft, 0, pt8211, 0);
AudioConnection      patchCord4(ampRight, 0, pt8211, 1);

float gainLvl = 0.5;

void setupAudio() {
    AudioMemory(8);

    ampLeft.gain(gainLvl);
    ampRight.gain(gainLvl);
}

uint32_t getAudioMS() {
    ampLeft.gain(0);
    ampRight.gain(0);

    char audioFile[8] = "";
    sprintf(audioFile, "%03d.WAV", getShowNumber());

    sdWav.play(audioFile);
    delay(1000);
    uint32_t audioMS = sdWav.lengthMillis();
    sdWav.stop();

    ampLeft.gain(gainLvl);
    ampRight.gain(gainLvl);

    return audioMS;
}

void playAudio() {
    char audioFile[8] = "";
    sprintf(audioFile, "%03d.WAV", getShowNumber());

    sdWav.play(audioFile);
}

void stopAudio() {
    sdWav.stop();
}
