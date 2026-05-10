#ifndef WOLFSENSE_H
#define WOLFSENSE_H

#include <Arduino.h>
#include <BLEDevice.h>

class WolfSenseClass
{
public:

  // Analog JOYSTICKS
    int JoyL_X = 0;
    int JoyL_Y = 0;

    int JoyR_X = 0;
    int JoyR_Y = 0;


  // BUTTONS
    bool A = 0;
    bool B = 0;
    bool X = 0;
    bool Y = 0;

    bool UP = 0;
    bool DOWN = 0;
    bool LEFT = 0;
    bool RIGHT = 0;

    bool LB = 0;
    bool RB = 0;
    bool LT = 0;
    bool RT = 0;

    bool START = 0;
    bool SELECT = 0;

    bool L3 = 0;
    bool R3 = 0;

    bool M1 = 0;
    bool M2 = 0;

    bool FN = 0;
    bool POWER = 0;


void SetMAC(const char* mac);

    void SetDeathZone(int dz);

    void begin();

    void loop();

    void PrintRawData();

    bool Connected();

private:

    BLEAddress* targetAddress = nullptr;

    BLEClient* client = nullptr;

    int deadzone = 0;

    uint8_t packetData[64];

    size_t packetLen = 0;

    volatile bool newPacket = false;

    static WolfSenseClass* instance;

    static void notifyCallback(
        BLERemoteCharacteristic* chr,
        uint8_t* data,
        size_t length,
        bool isNotify
    );

    bool connectBLE();

    int8_t normalizeJoyX(uint8_t v);

    int8_t normalizeJoyY(uint8_t v);

    void decodeButtons();

    int applyDeadzone(int v);
};

extern WolfSenseClass WolfSense;

#endif