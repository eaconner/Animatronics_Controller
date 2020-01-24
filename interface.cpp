/**
*   @file   interface.cpp
*   @brief  Functions for getting data from the serial buffer
*/

#include "interface.h"

char getChar() {
    while (Serial.available() == 0) {
        // Wait for Serial data
    }

    char value = Serial.read();
    Serial.println(value);

    while (Serial.available() > 0) {
        Serial.read();
    }

    return value;
}

uint32_t getInt() {
    while (Serial.available() == 0) {
        // Wait for Serial data
    }

    uint32_t value = constrain(Serial.parseInt(), 0, 4294967295);
    Serial.println(value);

    while (Serial.available() > 0) {
        Serial.read();
    }

    return value;
}

char* getString() {
    uint8_t c = 0;
    static char buffer[16];
    memset(buffer, 0, sizeof buffer);

    while (Serial.available() == 0) {
        // Wait for Serial data
    }

    while (Serial.available() > 0) {
        buffer[c] = Serial.read();
        c++;
    }

    Serial.println(buffer);

    return buffer;
}
