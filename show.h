/**
*   @file   show.h
*   @brief  Functions for creating, loading, saving, recording and playing a show file
*/

#ifndef SHOW_H_
    #define SHOW_H_

    #include <Arduino.h>

    /**
    *   @brief  Create a new show file, calls record after the file is created
    */
    void newShow(void);

    /**
    *   @brief  Load a given show file from SD card
    *
    *   @param  number  0 ... 255
    *   @return ```true``` if show loaded and ```false``` if there was an error
    */
    bool loadShow(uint8_t number);

    /**
    *   @brief  Save show file to SD card
    */
    void saveShow(void);

    /**
    *   @brief  Delete show file from SD card
    */
    void deleteShow(void);

    /**
    *   @brief  Play the loaded show
    */
    void playShow(void);

    /**
    *   @brief  Record show
    */
    void recordShow(void);

    /**
    *   @brief  Test servo function
    */
    void testShow(void);

    /**
    *   @brief  Get the loaded show number
    *
    *   @return Returns the loaded show number, 0 ... 255
    */
    uint8_t getShowNumber(void);

    /**
    *   @brief  Set the show number
    *
    *   @param  number  Show number, 0 ... 255
    */
    void setShowNumber(uint8_t number);

    /**
    *   @brief  Get the length of the show in milliseconds
    *
    *   @return Returns the show length in milliseconds, 0 ... 4294967295
    */
    uint32_t getShowMS(void);

    /**
    *   @brief  Set the length of the show in milliseconds
    *
    *   @param  ms  Show length in milliseconds, 0 ... 4294967295
    */
    void setShowMS(uint32_t ms);

    /**
    *   @brief  Get the show name
    *
    *   @return Returns the show name as a char[16]
    */
    char* getShowName(void);

    /**
    *   @brief  Set the show name
    *
    *   @param  name Name of show to write to file
    */
    void setShowName(char* name);

    /**
    *   @brief  Brief description
    *
    *   @param  address Address to write data to, 0x0000 ... 0xFFE0
    *   @param  data    Data to write, 0 ... 255
    */
    void saveData(uint32_t address, uint8_t data);

    /**
    *   @brief  Get data from show file
    *
    *   @param  address Address to read data from, 0x0000 ... 0xFFE0
    *   @return Returns data from address, 0 ... 255
    */
    uint8_t getData(uint32_t address);

    /**
    *   @brief  Get the current show frame
    *
    *   @return Returns the current show frame count, 0x0000 ... 0xFFE0
    */
    uint32_t getShowFrameCount(void);

    /**
    *   @brief  Get the max show frame
    *
    *   @return Returns the max show frame, 0x0000 ... 0xFFE0
    */
    uint32_t getShowMaxFrameCount(void);

#endif  // SHOW_H_
