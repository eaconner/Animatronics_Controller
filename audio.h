/**
*   @file   audio.h
*   @brief  Functions for playing audio with the PT8211
*/

#ifndef AUDIO_H_
    #define AUDIO_H_

    #include <Arduino.h>

    /**
    *   @brief  Setup audio output (Note: This is required)
    */
    void setupAudio(void);

    /**
    *   @brief  Get the length of WAV file associated with the loaded show in milliseconds
    *
    *   @return Returns the length of the loaded audio file in milliseconds, 0 ... 4294967295
    */
    uint32_t getAudioMS(void);

    /**
    *   @brief  Play WAV file associated with the loaded show
    */
    void playAudio(void);

    /**
    *   @brief  Stop playing WAV file
    */
    void stopAudio(void);

#endif  // AUDIO_H_
