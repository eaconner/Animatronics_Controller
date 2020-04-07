/**
*   @file   config.cpp
*   @brief  Functions for loading, saving and modifying the config file
*/

#include "config.h"
#include "interface.h"
#include "servo.h"
#include "show.h"
#include <SD.h>

#define CONF_BYTE_SIZE 0x30F
char configFile[8] = "FIG.CFG";
File CONFIG_FILE;
uint8_t conf[CONF_BYTE_SIZE] = {};

void loadConfig() {
    if (SD.exists(configFile)) {
        CONFIG_FILE = SD.open(configFile);

        if (CONFIG_FILE) {
            while (CONFIG_FILE.available()) {
                CONFIG_FILE.read(conf, sizeof(conf));
            }

            CONFIG_FILE.close();
        } else {
            Serial.print("Error opening: ");
            Serial.println(configFile);
        }
    } else {
        Serial.print("File ");
        Serial.print(configFile);
        Serial.println(" does not exist");
    }
}

void saveConfig() {
    if (SD.exists(configFile)) {
        Serial.print("Show ");
        Serial.print(configFile);
        Serial.print(" already exists. Overwrite? 'y' or 'n' ");

        switch (getChar()) {
            case 'y':
                SD.remove(configFile);
                break;
            default:
                return;
                break;
        }
    }

    CONFIG_FILE = SD.open(configFile, FILE_WRITE);

    if (CONFIG_FILE) {
        CONFIG_FILE.write(conf, sizeof(conf));
        CONFIG_FILE.close();
		setupServos();
    } else {
        Serial.print("Error opening: ");
        Serial.println(configFile);
    }
}

char* getFigureName() {
    static char name[17];
    memset(name, 0, sizeof name);

    for (uint8_t c = 0; c <= 16; c++) {
        if (conf[c] != 0x00) {
            name[c] = conf[c];
        }
    }

    return name;
}

char* getInputName(uint8_t number) {
    uint16_t inputBase = 0x108 + (number * 0x10);
    static char name[9];
    memset(name, 0, sizeof name);

    for (uint8_t c = 0; c <= 8; c++) {
        if (conf[inputBase + c] != 0x00) {
            name[c] = conf[inputBase + c];
        }
    }

    return name;
}

void setInputName(uint8_t number, char* name) {
    uint16_t inputBase = 0x108 + (number * 0x10);

    for (uint8_t c = 0; c <= 8; c++) {
        if (name[c] != 0x00) {
            conf[inputBase + c] = name[c];
        }
    }
}

char* getServoName(uint8_t number) {
    uint16_t servoBase = 0x208 + (number * 0x10);
    static char name[9];
    memset(name, 0, sizeof name);

    for (uint8_t c = 0; c <= 8; c++) {
        if (conf[servoBase + c] != 0x00) {
            name[c] = conf[servoBase + c];
        }
    }

    return name;
}

void setServoName(uint8_t number, char* name) {
    uint16_t servoBase = 0x208 + (number * 0x10);

    for (uint8_t c = 0; c <= 8; c++) {
        if (name[c] != 0x00) {
            conf[servoBase + c] = name[c];
        }
    }
}

input_t getInputData(uint8_t number) {
    input_t i;
    uint16_t inputBase = 0x100 + (number * 0x10);

    i.enabled = conf[inputBase];
    i.pin = conf[inputBase + 1];
    i.min = (conf[inputBase + 3] << 8) + conf[inputBase + 2];
    i.max = (conf[inputBase + 5] << 8) + conf[inputBase + 4];
    i.value = 0;

    return i;
}

servo_t getServoData(uint8_t number) {
    servo_t s;
    uint16_t servoBase = 0x200 + (number * 0x10);
	uint16_t servoFilterBase = 0x300 + number;

    s.enabled = conf[servoBase];
    s.pin = conf[servoBase + 1];
    s.min = (conf[servoBase + 3] << 8) + conf[servoBase + 2];
    s.max = (conf[servoBase + 5] << 8) + conf[servoBase + 4];
    s.input = getInputData(conf[servoBase + 6]);
    s.invert = conf[servoBase + 7];
    s.value = 0;
	s.filter = conf[servoFilterBase];

    return s;
}

uint16_t getServoCenter(uint8_t number) {
    uint16_t servoBase = 0x200 + (number * 0x10);

    uint16_t min = (conf[servoBase + 3] << 8) + conf[servoBase + 2];
    uint16_t max = (conf[servoBase + 5] << 8) + conf[servoBase + 4];

    uint16_t center = ((max - min) / 2) + min;

    return center;
}

uint16_t* minmaxInput(uint8_t input) {
    uint16_t inputBase = 0x100 + (input * 0x10);
    uint16_t potValue = analogRead(conf[inputBase + 1]);
    uint16_t potValuePrev = potValue;
    uint16_t potMin = potValue;
    uint16_t potMax = potValue;
    uint32_t millisNow = 0;
    uint32_t millisPrev = 0;

    while (Serial.available() <= 0) {
        millisNow = millis();

        if (millisNow - millisPrev >= 40) {
            millisPrev = millisNow;

            potValue = analogRead(conf[inputBase + 1]);
            if (potValue != potValuePrev) {
                potValuePrev = potValue;

                if (potValue < potMin) {
                    potMin = potValue;
                }

                if (potValue > potMax) {
                    potMax = potValue;
                }

                Serial.print("Min: ");
                Serial.print(potMin);
                Serial.print(" | Max: ");
                Serial.println(potMax);
            }
        }
    }

    while (Serial.available() > 0) {
        Serial.read();
    }

    static uint16_t values[2] = {potMin, potMax};
    return values;
}

void configInput(uint8_t number) {
    uint16_t inputBase = 0x100 + (number * 0x10);
    uint8_t teensyPins[16] = {0, 1, 2, 3, 6, 7, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};

    Serial.print("---- Configure Input #");
    Serial.print(number);
    Serial.println(" ----");
    Serial.println(getInputName(number));

    conf[inputBase + 1] = teensyPins[number];  // Pin

    Serial.print("\nEnabled 0-1: ");
    conf[inputBase] = getInt();  // Enabled

    Serial.println("Set Min / Max for input, 'a' to accept...");
    uint16_t* MinMax = minmaxInput(conf[inputBase + 1]);
    uint16_t potMin = MinMax[0];
    uint16_t potMax = MinMax[1];
    conf[inputBase + 3] = (potMin & 0xFF00) >> 8;  // Min High
    conf[inputBase + 2] = (potMin & 0x00FF);  // Min Low
    conf[inputBase + 5] = (potMax & 0xFF00) >> 8;  // Max High
    conf[inputBase + 4] = (potMax & 0x00FF);  // Max Low

    processInputs();
}

void configServo(uint8_t number) {
    uint16_t servoBase = 0x200 + (number * 0x10);

    Serial.print("\n---- Configure Servo #");
    Serial.print(number);
    Serial.println(" ----");
    Serial.println(getServoName(number));

    conf[servoBase + 1] = number;  // Pin

    Serial.print("\nEnabled 0-1: ");
    conf[servoBase] = getInt();  // Enabled

    printInputs();
	Serial.print("Which input would you like to use 0-15: ");
    conf[servoBase + 6] = getInt();  // Input
    Serial.println(getInputName(conf[servoBase + 6]));

    Serial.print("Invert 0-1: ");
    conf[servoBase + 7] = getInt();  // Invert

    Serial.println("\n\nSet Min for servo, 'a' to accept...");
    uint16_t min = minmaxServo(conf[servoBase + 6], conf[servoBase + 1]);
    conf[servoBase + 3] = (min & 0xFF00) >> 8;  // Min High
    conf[servoBase + 2] = (min & 0x00FF);  // Min Low

    Serial.println("\n\nSet Max for servo, 'a' to accept...");
    uint16_t max = minmaxServo(conf[servoBase + 6], conf[servoBase + 1]);
    conf[servoBase + 5] = (max & 0xFF00) >> 8;  // Max High
    conf[servoBase + 4] = (max & 0x00FF);  // Max Low

    processServos();
}

void invertServo(uint8_t number) {
    uint16_t servoBase = 0x200 + (number * 0x10);
    uint8_t value = conf[servoBase + 7];

    if (value == 1) {
        conf[servoBase + 7] = 0;
    } else {
        conf[servoBase + 7] = 1;
    }

    Serial.print("Servo #");
    Serial.print(number);
    Serial.println(" inverted");
}

void filterServo(uint8_t number) {
    uint16_t servoFilterBase = 0x300 + number;
    uint8_t currentValue = conf[servoFilterBase];

    Serial.print("---- Configure Servo #");
    Serial.print(number);
    Serial.println(" Filter ----");
    Serial.println(getServoName(number));
	
	Serial.print("Current filter value: ");
	Serial.println(currentValue);
	
	Serial.print("Set new filter value: ");
	uint8_t newValue = getInt();

    conf[servoFilterBase] = newValue;
}

void toggleServo(uint8_t number) {
    uint16_t servoBase = 0x200 + (number * 0x10);
    uint8_t value = conf[servoBase];

    Serial.print("Servo #");
    Serial.print(number);

    if (value == 1) {
        conf[servoBase + 7] = 0;
        Serial.println(" disabled");
    } else {
        conf[servoBase + 7] = 1;
        Serial.println(" enabled");
    }
}

void printServos() {
	Serial.println();
	for (int s = 0; s < 16; s++ ) {
		uint16_t servoBase = 0x200 + (s * 0x10);
		uint8_t enabled = conf[servoBase];
		if (enabled) {
			Serial.print(s);
			Serial.print(": ");
			Serial.println(getServoName(s));
		}
	}
}

void printInputs() {
	Serial.println();
	for (int i = 0; i < 16; i++ ) {
		uint16_t inputBase = 0x100 + (i * 0x10);
		uint8_t enabled = conf[inputBase];
		if (enabled) {
			Serial.print(i);
			Serial.print(": ");
			Serial.println(getInputName(i));
		}
	}
}
