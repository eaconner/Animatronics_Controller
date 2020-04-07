/**
*   @file   show.cpp
*   @brief  Functions for creating, loading, saving, recording and playing a show file from the SD card.
*/

#include "show.h"
#include "audio.h"
#include "config.h"
#include "interface.h"
#include "servo.h"
#include <SD.h>

#define SAMPLE_RATE 10  // 10fps 100ms | 20fps 50ms | 25fps 40ms | 40fps 25ms | 50fps 20ms
#define SHOW_BYTE_SIZE 0xFFFF
char fileName[8] = "";
File SHOW_FILE;
uint8_t program[SHOW_BYTE_SIZE + 1] = {};
uint32_t showFrameCount = 0;
uint32_t showServoMaxFrameCount = 0;
uint32_t showMaxFrameCount = 0;
uint32_t millisNow = 0;
uint32_t millisPrev = 0;

void newShow() {
    program[0xFFE8] = 0xA9;
    program[0xFFE9] = 0x32;
    program[0xFFEA] = 0x30;
    program[0xFFEB] = 0x31;
    program[0xFFEC] = 0x39;
    program[0xFFED] = 0x20;
    program[0xFFEE] = 0x43;
    program[0xFFEF] = 0x43;

    Serial.print("\nEnter show number 0-255: ");
    setShowNumber(getInt());

    Serial.print("Enter show name: ");
    setShowName(getString());

    sprintf(fileName, "%03d.WAV", getShowNumber());

    if (SD.exists(fileName)) {
        setupAudio();
        uint32_t audioMS = getAudioMS();
        Serial.print(fileName);
        Serial.print(" length in milliseconds: ");
        Serial.println(audioMS);
        setShowMS(audioMS);
    } else {
        Serial.print("Enter show length in milliseconds: ");
        setShowMS(getInt());
    }

    showMaxFrameCount = getShowMS() / SAMPLE_RATE;

    if ((showMaxFrameCount * getInputCount()) > (SHOW_BYTE_SIZE - 0x20)) {
        Serial.println("Record time is too long");
    }

    Serial.println("\nRecording in...");
    delay(1000);
    Serial.println("3...");
    delay(1000);
    Serial.println("2...");
    delay(1000);
    Serial.println("1...");
    delay(1000);

    recordShow();
}

bool loadShow(uint8_t number) {
    sprintf(fileName, "%03d.ANI", number);

    if (SD.exists(fileName)) {
        SHOW_FILE = SD.open(fileName);

        if (SHOW_FILE) {
            while (SHOW_FILE.available()) {
                SHOW_FILE.read(program, sizeof(program));
            }

            SHOW_FILE.close();

            Serial.print("Loaded: ");
            Serial.println(fileName);

            showMaxFrameCount = getShowMS() / SAMPLE_RATE;

            if ((showMaxFrameCount * getInputCount()) > (SHOW_BYTE_SIZE - 0x20)) {
                Serial.println("Record time is too long");
                return false;
            }

            delay(100);

            return true;
        } else {
            Serial.print("Error opening: ");
            Serial.println(fileName);
            return false;
        }
    } else {
        Serial.print("Show ");
        Serial.print(fileName);
        Serial.println(" does not exist");
        return false;
    }
}

void saveShow() {
    sprintf(fileName, "%03d.ANI", getShowNumber());

    if (SD.exists(fileName)) {
        Serial.print("Show ");
        Serial.print(fileName);
        Serial.print(" already exists. Overwrite? 'y' or 'n' ");

        switch (getChar()) {
            case 'y':
                SD.remove(fileName);
                break;
            default:
                break;
        }
    }

    SHOW_FILE = SD.open(fileName, FILE_WRITE);

    if (SHOW_FILE) {
        SHOW_FILE.write(program, sizeof(program));
        SHOW_FILE.close();
    } else {
        Serial.print("Error opening: ");
        Serial.println(fileName);
    }
}

void deleteShow() {
    sprintf(fileName, "%03d.ANI", getShowNumber());

    if (SD.exists(fileName)) {
        Serial.print("Delete show ");
        Serial.print(fileName);
        Serial.print("? 'y' or 'n' ");

        switch (getChar()) {
            case 'y':
                SD.remove(fileName);
                break;
            default:
                break;
        }
    }
}

void playShow() {
    showFrameCount = 0;
    uint8_t servoCount = getServoCount();

    playAudio();

    while (showFrameCount < showMaxFrameCount) {
        millisNow = millis();

        if (millisNow - millisPrev >= SAMPLE_RATE) {
            millisPrev = millisNow;

            for (uint8_t s = 0; s < servoCount; s++) {
                playServo(s);
            }

            showFrameCount++;
        }
    }

    delay(100);
}

void recordShow() {
    showFrameCount = 0;
    uint8_t servoCount = getServoCount();

    playAudio();

    while (showFrameCount < showMaxFrameCount) {
        millisNow = millis();

        if (millisNow - millisPrev >= SAMPLE_RATE) {
            millisPrev = millisNow;

            for (uint8_t s = 0; s < servoCount; s++) {
                recordServo(s);
            }

            showFrameCount++;
        }
    }
}

void testShow() {
    Serial.println("Starting test, 'e' to exit...");
	uint8_t servoCount = getServoCount();
	
    while (Serial.available() <= 0) {
        millisNow = millis();
        
        if (millisNow - millisPrev >= SAMPLE_RATE) {
            millisPrev = millisNow;

            for (uint8_t s = 0; s < servoCount; s++) {
                updateServo(s);
            }
        }
    }

    while (Serial.available() > 0) {
        Serial.read();
    }
}

uint8_t getShowNumber() {
    return program[0xFFE0];
}

void setShowNumber(uint8_t number) {
    program[0xFFE0] = number;
}

uint32_t getShowMS() {
    return (program[0xFFE4] << 24) + (program[0xFFE3] << 16) + (program[0xFFE2] << 8) + program[0xFFE1];
}

void setShowMS(uint32_t ms) {
    program[0xFFE4] = (ms & 0xFF000000UL) >> 24;
    program[0xFFE3] = (ms & 0x00FF0000UL) >> 16;
    program[0xFFE2] = (ms & 0x0000FF00UL) >> 8;
    program[0xFFE1] = (ms & 0x000000FFUL);
}

char* getShowName() {
    static char name[16];
    memset(name, 0, sizeof name);

    for (uint8_t c = 0; c < 15; c++) {
        if (program[0xFFF0 + c] != 0x00) {
            name[c] = program[0xFFF0 + c];
        }
    }

    return name;
}

void setShowName(char* name) {
    for (uint8_t c = 0; c < 16; c++) {
        if (name[c] != 0x00) {
            program[0xFFF0 + c] = name[c];
        } else {
        	program[0xFFF0 + c] = 0x00;
        }
    }

	saveShow();
}

void saveData(uint32_t address, uint8_t data) {
    program[address] = data;
}

uint8_t getData(uint32_t address) {
    return program[address];
}

uint32_t getShowFrameCount() {
    return showFrameCount;
}

uint32_t getShowMaxFrameCount() {
    return showMaxFrameCount;
}
