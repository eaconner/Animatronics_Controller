/**
*   @file   servo.cpp
*   @brief  Functions for working with the servos
*/

#include "servo.h"
#include "config.h"
#include "interface.h"
#include "show.h"
#include <PWM_Servo.h>

PWMServo servoBoard = PWMServo();  // Use default address 0x40

input_t input[16];
servo_t servo[16];
float servoFilterValue[16];

void setupServos() {
    servoBoard.begin();
    servoBoard.setPWMFreq(60);

    loadConfig();

    processInputs();
    processServos();

    centerServos();
}

void processInputs() {
    for (uint8_t i = 0; i < 16; i++) {
        input[i] = getInputData(i);
    }
}

uint8_t getInputCount() {
    uint8_t count = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if (input[i].enabled != 0) {
            count++;
        }
    }

    return count;
}

void processServos() {
    for (uint8_t s = 0; s < 16; s++) {
        servo[s] = getServoData(s);
    }
}

uint8_t getServoCount() {
    uint8_t count = 0;

    for (uint8_t s = 0; s < 16; s++) {
        if (servo[s].enabled != 0) {
            count++;
        }
    }

    return count;
}

void centerServos() {
    uint8_t servoCount = getServoCount();

    for (uint8_t s = 0; s < servoCount; s++) {
		if (servo[s].enabled) {
            servoBoard.setPin(s, getServoCenter(s));
			servoFilterValue[s] = map(getServoCenter(s), 0, 255, servo[s].max, servo[s].min);
        }
    }
}

void updateServo(uint8_t number) {
    servo_t s = servo[number];

    if (s.enabled) {
        s.input.value = analogRead(s.input.pin);
        s.input.value = constrain(s.input.value, s.input.min, s.input.max);
        s.input.value = map(s.input.value, s.input.min, s.input.max, 0, 255);
		servoFilterValue[number] = filter(servoFilterValue[number], s.input.value, s.filter);

        if (s.invert) {
            s.value = map(servoFilterValue[number], 0, 255, s.max, s.min);
        } else {
            s.value = map(servoFilterValue[number], 0, 255, s.min, s.max);
        }

        servoBoard.setPin(s.pin, s.value);
    }
}

uint16_t minmaxServo(uint8_t pin, uint8_t servo) {
    input_t i = input[pin];
    uint16_t servoValue = analogRead(i.pin);
    uint16_t servoValuePrev = servoValue;
    uint32_t millisNow = 0;
    uint32_t millisPrev = 0;

    while (Serial.available() <= 0) {
        millisNow = millis();

        if (millisNow - millisPrev >= 40) {
            millisPrev = millisNow;

            i.value = analogRead(i.pin);
            i.value = constrain(i.value, i.min, i.max);
            i.value = map(i.value, i.min, i.max, 0, 255);

            servoValue = map(i.value, 0, 255, 0, 800);

            if (servoValue != servoValuePrev) {
                servoValuePrev = servoValue;

                servoBoard.setPin(servo, servoValue);
                Serial.print("Servo Position: ");
                Serial.println(servoValue);
            }
        }
    }

    while (Serial.available() > 0) {
        Serial.read();
    }

    uint8_t multiple = 5;
    return ((servoValue + multiple/2) / multiple) * multiple;
}

void recordServo(uint8_t number) {
    servo_t s = servo[number];

    if (s.enabled) {
        s.input.value = analogRead(s.input.pin);
        s.input.value = constrain(s.input.value, s.input.min, s.input.max);
        s.input.value = map(s.input.value, s.input.min, s.input.max, 0, 255);
        saveData(getShowFrameCount() + (getShowMaxFrameCount() * number), s.input.value);

        if (s.invert) {
            s.value = map(s.input.value, 0, 255, s.max, s.min);
			servoFilterValue[number] = filter(servoFilterValue[number], s.value, s.filter);
        } else {
            s.value = map(s.input.value, 0, 255, s.min, s.max);
			servoFilterValue[number] = filter(servoFilterValue[number], s.value, s.filter);
        }
		
        servoBoard.setPin(s.pin, servoFilterValue[number]);
    }
}

void playServo(uint8_t number) {
    servo_t s = servo[number];

    if (s.enabled) {
        s.input.value = getData(getShowFrameCount() + (getShowMaxFrameCount() * number));

        if (s.invert) {
            s.value = map(s.input.value, 0, 255, s.max, s.min);
			servoFilterValue[number] = filter(servoFilterValue[number], s.value, s.filter);
        } else {
            s.value = map(s.input.value, 0, 255, s.min, s.max);
			servoFilterValue[number] = filter(servoFilterValue[number], s.value, s.filter);
        }
		
        servoBoard.setPin(s.pin, servoFilterValue[number]);
    }
}

float filter(float servoValue, float inputValue, uint8_t filter) {
	float lengthFiltered = (inputValue + (servoValue * filter)) / (filter + 1);
	return lengthFiltered;  
}
