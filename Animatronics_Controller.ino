/**
*   @file   Animatronics_Controller.ino
*   @brief  A custom Animatronics Controller with the ability to load and save configuration and show files from an SD card in sync with a 16-bit WAV file
*/

#include "audio.h"
#include "config.h"
#include "interface.h"
#include "servo.h"
#include "show.h"
#include <SD.h>

#define INTERFACE_PIN 28
#define TEST_PIN 29

char versionNumber[] = "2.2.1";

/**
*   @brief  Setup the Animatronics Controller
*/
void setup() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        while (!SD.begin(BUILTIN_SDCARD)) {
            // Wait for SD Card
        }
    }

    setupServos();
    setupAudio();

    pinMode(INTERFACE_PIN, INPUT);
    bool interfaceButton = digitalRead(INTERFACE_PIN);

    pinMode(TEST_PIN, INPUT);
    bool testButton = digitalRead(TEST_PIN);

    if (interfaceButton && !testButton) {
        Serial.begin(9600);
        while (!Serial) {
            // Wait for Serial
        }

        Serial.println("-----------------------------------");
        Serial.println("----- Animatronics Controller -----");
        Serial.print("--------- Version: ");
        Serial.print(versionNumber);
        Serial.println(" ----------");
        Serial.println("-----------------------------------\n");
        Serial.println(getFigureName());

        mainMenu();
    }

    if (testButton && !interfaceButton) {
        testShow();
    }

    if (testButton && interfaceButton) {
        // Not used
    }
}

/**
*   @brief  Main program loop, load each show file and play it
*/
void loop() {
    char fileName[8] = "";

    for (uint8_t s = 0; s < 256; s++) {
        sprintf(fileName, "%03d.ANI", s);

        if (SD.exists(fileName)) {
            if (loadShow(s)) {
                playShow();
            }
        } else {
            delay(10);
        }
    }
}

/**
*   @brief  Main menu
*/
void mainMenu() {
    Serial.println("\n-------------------------------");
    Serial.println("n - New Show File");
    Serial.println("l - Load Show File");
    Serial.println("a - Auto Play");
    Serial.println("t - Test");
    Serial.println("c - Configure");
    Serial.println("-------------------------------\n");
    Serial.print("Select an option: ");

    switch (getChar()) {
        case 'n':
            newShow();
            loadedShowMenu();
            break;
        case 'l':
            Serial.print("\nEnter show number 0-255: ");
            if (loadShow(getInt())) {
                loadedShowMenu();
            }
            break;
        case 'a':
            loop();
            break;
        case 't':
            testShow();
            break;
        case 'c':
            configMenu();
            break;
        default:
            Serial.println("Invalid value...\n");
            break;
    }

    mainMenu();
}

/**
*   @brief  Loaded show menu
*/
void loadedShowMenu() {
    Serial.print("\nLoaded Show #");
    Serial.print(getShowNumber());
    Serial.print(" | ");
    Serial.println(getShowName());
    Serial.println("-------------------------------");
    Serial.println("p - Play Show File");
    Serial.println("l - Loop Show File");
    Serial.println("r - Record Show File");
    Serial.println("s - Save Show File");
    Serial.println("d - Delete Show File");
    Serial.println("e - Exit");
    Serial.println("-------------------------------\n");
    Serial.print("Select an option: ");

    switch (getChar()) {
        case 'p':
            playShow();
            break;
        case 'l':
            while (true) {
                playShow();
            }
            break;
        case 'r':
            Serial.println("\nRecording in...");
            delay(1000);
            Serial.println("3...");
            delay(1000);
            Serial.println("2...");
            delay(1000);
            Serial.println("1...");
            delay(1000);

            recordShow();
            break;
        case 's':
            saveShow();
            break;
        case 'd':
            deleteShow();
            mainMenu();
            break;
        case 'e':
            mainMenu();
            break;
        default:
            Serial.println("Invalid value...\n");
            break;
    }

    loadedShowMenu();
}

/**
*   @brief  Config menu
*/
void configMenu() {
    Serial.println("\n-------------------------------");
    Serial.println("i - Config Input");
    Serial.println("c - Config Servo");
    Serial.println("x - Invert Servo");
	Serial.println("f - Servo Filter");
    Serial.println("d - Enable/Disable Servo");
    Serial.println("s - Save Config File");
    Serial.println("t - Test");
    Serial.println("e - Exit");
    Serial.println("-------------------------------\n");
    Serial.print("Select an option: ");

    switch (getChar()) {
        case 'i':
            Serial.print("\nEnter input number 0-15: ");
            configInput(getInt());
            break;
        case 'c':
            Serial.print("\nEnter servo number 0-15: ");
            configServo(getInt());
            break;
        case 'x':
            Serial.print("\nEnter servo number 0-15: ");
            invertServo(getInt());
            break;
	    case 'f':
	        Serial.print("\nEnter servo number 0-15: ");
	        filterServo(getInt());
	        break;
        case 'd':
            Serial.print("\nEnter servo number 0-15: ");
            toggleServo(getInt());
            break;
        case 's':
            saveConfig();
            break;
        case 't':
            testShow();
            break;
        case 'e':
            mainMenu();
            break;
        default:
            Serial.println("Invalid value...\n");
            break;
    }

    configMenu();
}
