/**
*   @file   servo.h
*   @brief  Functions for working with the servos
*/

#ifndef SERVO_H_
    #define SERVO_H_

    #include <Arduino.h>

    /**
    *   @brief  Struct for Input settings
    */
    struct input_t {
        bool enabled;
        uint8_t pin;
        uint16_t min;
        uint16_t max;
        uint16_t value;
    };

    /**
    *   @brief  Struct for Servo settings
    */
    struct servo_t {
        bool enabled;
        uint8_t pin;
        uint16_t min;
        uint16_t max;
        uint16_t value;
		uint8_t filter;
        input_t input;
        bool invert;
    };

    /**
    *   @brief  Setup the servo output (Note: This is required)
    */
    void setupServos(void);

    /**
    *   @brief  Load input data from the config file to an array
    */
    void processInputs(void);

    /**
    *   @brief  Get the total number of inputs from the config file
    *
    *   @return Returns the total number of inputs, 0 ... 15
    */
    uint8_t getInputCount(void);

    /**
    *   @brief  Load servo data from the config file to an array
    */
    void processServos(void);

    /**
    *   @brief  Get the total number of servos from the config file
    *
    *   @return Returns the total number of servos, 0 ... 15
    */
    uint8_t getServoCount(void);

    /**
    *   @brief  Move all enabled servos to the center position
    */
    void centerServos(void);

    /**
    *   @brief  Read a given servo input and update its position
    *
    *   @param  number  Servo number to update, 0 ... 15
    */
    void updateServo(uint8_t number);

    /**
    *   @brief  Configure servo Min/Max
    *
    *   @param  pin Input pin to read, 0 ... 15
    *   @param  servo   Servo pin to write, 0 ... 15
    *   @return Returns the servo position as uint16_t, 150 ... 600
    */
    uint16_t minmaxServo(uint8_t pin, uint8_t servo);

    /**
    *   @brief  Read a given servo input, save it to the show file and update its position
    *
    *   @param  number  Servo number to record, 0 ... 15
    */
    void recordServo(uint8_t number);

    /**
    *   @brief  Read a given servo from the show file and update its position
    *
    *   @param  number  Servo number to play, 0 ... 15
    */
    void playServo(uint8_t number);
	
    /**
    *   @brief  Filter servo value for smoothing
    *
    *   @param  prevValue  @Todo
	*	@param  currentValue  @Todo
	*	@param  filter  @Todo
	*   @return Returns filtered servo value
    */
	uint16_t filter(uint16_t prevValue, uint16_t currentValue, int filter);

#endif  // SERVO_H_
