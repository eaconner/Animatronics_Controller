/**
*   @file   interface.h
*   @brief  Functions for getting data from the serial buffer
*/

#ifndef INTERFACE_H_
    #define INTERFACE_H_

    #include <Arduino.h>

    /**
    *   @brief  Get a char from the serial buffer
    *
    *   @return Returns a char from the serial buffer
    */
    char getChar(void);

    /**
    *   @brief  Get a uint32_t from the serial buffer
    *
    *   @return Returns a uint32_t from the serial buffer
    */
    uint32_t getInt(void);

    /**
    *   @brief  Get char[16] from the serial buffer
    *
    *   @return Returns a char[16] from the serial buffer
    */
    char* getString(void);

#endif  // INTERFACE_H_
