/**
*   @file   config.h
*   @brief  Functions for loading, saving and modifying the config file
*/

#ifndef CONFIG_H_
    #define CONFIG_H_

    #include "servo.h"
    #include <Arduino.h>

    /**
    *   @brief  Load the config file from SD card
    */
    void loadConfig(void);

    /**
    *   @brief  Save the config file to the SD card
    */
    void saveConfig(void);

    /**
    *   @brief  Get the figure name from the config file
    *
    *   @return Returns the figure name as a char[16]
    */
    char* getFigureName(void);

    /**
    *   @brief  Get the input name for a given number from the config file
    *
    *   @param  number  Input number, 0 ... 15
    *   @return Returns the input name for a given number as a char[8]
    */
    char* getInputName(uint8_t number);

    /**
    *   @brief  Set the input name for a given number from the config file
    *
    *   @param  number  Input number, 0 ... 15
    *   @param  name    Input name, char[8]
    */
    void setInputName(uint8_t number, char* name);

    /**
    *   @brief  Get the servo name for a given number from the config file
    *
    *   @param  number  Servo number, 0 ... 15
    *   @return Returns the servo name for a given number as a char[8]
    */
    char* getServoName(uint8_t number);

    /**
    *   @brief  Set the servo name for a given number from the config file
    *
    *   @param  number  Servo number, 0 ... 15
    *   @param  name    Servo name, char[8]
    */
    void setServoName(uint8_t number, char* name);

    /**
    *   @brief  Get the input data for a given input number
    *
    *   @param  number  Input number, 0 ... 15
    *   @return Returns a input_t struct
    */
    input_t getInputData(uint8_t number);

    /**
    *   @brief  Get the servo data for a given servo number
    *
    *   @param  number  Servo number, 0 ... 15
    *   @return Returns a servo_t struct
    */
    servo_t getServoData(uint8_t number);

    /**
    *   @brief  Get the center position for a given servo
    *
    *   @param  number  Servo number, 0 ... 15
    *   @return Returns a uint16_t for the center position of a given servo
    */
    uint16_t getServoCenter(uint8_t number);

    /**
    *   @brief  Configure Input Min / Max
    *
    *   @param  input   Input pin to read, 0 ... 15
    *   @return Returns the Input Min / Max as uint16_t array, 0 ... 1023
    */
    uint16_t* minmaxInput(uint8_t input);

    /**
    *   @brief  Configure a given input
    *
    *   @param  number  Input number, 0 ... 15
    */
    void configInput(uint8_t number);

    /**
    *   @brief  Configure a given servo
    *
    *   @param  number  Servo number, 0 ... 15
    */
    void configServo(uint8_t number);

    /**
    *   @brief  Invert a given servo
    *
    *   @param  number  Servo number, 0 ... 15
    */
    void invertServo(uint8_t number);
	
    /**
    *   @brief  Change filter value for a given servo
    *
    *   @param  number  Servo number, 0 ... 15
    */
	void filterServo(uint8_t number);

    /**
    *   @brief  Enable/Disable a given servo
    *
    *   @param  number  Servo number, 0 ... 15
    */
    void toggleServo(uint8_t number);
	
	void printServos(void);
	void printInputs(void);

#endif  // CONFIG_H_
