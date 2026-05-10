#include "WolfSense.h"

WolfSenseClass WolfSense;

WolfSenseClass* WolfSenseClass::instance = nullptr;



// SET MAC
void WolfSenseClass::SetMAC(const char* mac)
{
    targetAddress = new BLEAddress(mac);
}



// DEADZONE ADJUST
void WolfSenseClass::SetDeathZone(int dz)
{
    deadzone = dz;
}


// ============================================================
// NORMALIZE X
//
// LEFT  = -127
// RIGHT = +127
// ============================================================

int8_t WolfSenseClass::normalizeJoyX(uint8_t v)
{
    if (v == 255)
        return 0;

    if (v <= 127)
        return v;

    return -(255 - v);
}


// ============================================================
// NORMALIZE Y
//
// UP    = +127
// DOWN  = -127
// ============================================================

int8_t WolfSenseClass::normalizeJoyY(uint8_t v)
{
    int16_t y;
    if (v == 255)
    {
        y = 0;
    }
    else if (v <= 127)
    {
        y = v;
    }
    else
    {
        y = -(255 - v);
    }

    y = -y;
    return y;
}


// APPLY DEADZONE
int WolfSenseClass::applyDeadzone(int v)
{
    if (abs(v) < deadzone)
        return 0;

    return v;
}


// CALLBACK
void WolfSenseClass::notifyCallback(
    BLERemoteCharacteristic* chr,
    uint8_t* data,
    size_t length,
    bool isNotify)
{
    if (!instance)
        return;

    if (length > 64)
        length = 64;

    memcpy(instance->packetData, data, length);

    instance->packetLen = length;


    // JOYSTICK
    if (length >= 1)
        instance->JoyL_X =
            instance->applyDeadzone(
                instance->normalizeJoyX(data[0]));

    if (length >= 2)
        instance->JoyL_Y =
            instance->applyDeadzone(
                instance->normalizeJoyY(data[1]));

    if (length >= 3)
        instance->JoyR_X =
            instance->applyDeadzone(
                instance->normalizeJoyX(data[2]));

    if (length >= 4)
        instance->JoyR_Y =
            instance->applyDeadzone(
                instance->normalizeJoyY(data[3]));


    
    // BUTTON
    instance->decodeButtons();

    instance->newPacket = true;
}


// DECODE BUTTONS
void WolfSenseClass::decodeButtons()
{
    // CLEAR ALL
    A = B = X = Y = 0;

    UP = DOWN = LEFT = RIGHT = 0;

    LB = RB = LT = RT = 0;

    START = SELECT = 0;

    L3 = R3 = 0;

    M1 = M2 = 0;

    FN = POWER = 0;


    // ========================================================
    // BYTE 8
    // ========================================================

    if (packetLen > 8)
    {
        uint8_t b = packetData[8];


        // ====================================================
        // ABXY
        // ====================================================

        Y = b & 128;
        X = b & 64;
        B = b & 32;
        A = b & 16;


        // ====================================================
        // DPAD
        // ====================================================

        uint8_t dpad = b & 0x0F;

        switch (dpad)
        {
            case 1:
                UP = 1;
                break;

            case 2:
                UP = 1;
                RIGHT = 1;
                break;

            case 3:
                RIGHT = 1;
                break;

            case 4:
                DOWN = 1;
                RIGHT = 1;
                break;

            case 5:
                DOWN = 1;
                break;

            case 6:
                DOWN = 1;
                LEFT = 1;
                break;

            case 7:
                LEFT = 1;
                break;

            case 8:
                UP = 1;
                LEFT = 1;
                break;
        }
    }


    // ========================================================
    // BYTE 9
    // ========================================================

    if (packetLen > 9)
    {
        uint8_t b = packetData[9];


        // ====================================================
        // SHOULDER
        // ====================================================

        LB = b & 1;
        RB = b & 2;

        LT = b & 4;
        RT = b & 8;


        // ====================================================
        // FIX START / SELECT
        // ====================================================

        START  = b & 32;
        SELECT = b & 16;

        L3 = b & 64;
        R3 = b & 128;
    }


    // ========================================================
    // BYTE 10
    // ========================================================

    if (packetLen > 10)
    {
        uint8_t b = packetData[10];


        // ====================================================
        // MACRO BUTTON
        // ====================================================

        M1 = b & 4;
        M2 = b & 8;

        FN = b & 128;
    }


    // ========================================================
    // BYTE 11
    // ========================================================

    if (packetLen > 11)
    {
        POWER = packetData[11] & 128;
    }
}


// ============================================================
// CONNECT BLE
// ============================================================

bool WolfSenseClass::connectBLE()
{
    Serial.println("Connecting...");
    client = BLEDevice::createClient();

    if (!client->connect(*targetAddress))
    {
        Serial.println("Connect failed!");
        return false;
    }

    Serial.println("CONNECTED!");


    // ========================================================
    // SERVICE SCAN
    // ========================================================

    auto* services = client->getServices();

    for (auto const& s : *services)
    {
        auto* service = s.second;
        auto* chars = service->getCharacteristics();

        for (auto const& c : *chars)
        {
            auto* characteristic = c.second;

            if (characteristic->canNotify())
            {
                Serial.print("Notify Char: ");
                Serial.println(
                    characteristic->getUUID()
                    .toString()
                    .c_str()
                );

                characteristic->registerForNotify(
                    notifyCallback
                );
            }
        }
    }

    return true;
}


// BEGIN
void WolfSenseClass::begin()
{
    instance = this;

    BLEDevice::init("WolfSense");

    connectBLE();
}


// LOOP
void WolfSenseClass::loop()
{
    if (!client)
        return;

    if (!client->isConnected())
    {
        Serial.println("Disconnected!");

        delay(2000);

        connectBLE();
    }
}


// CONNECTED
bool WolfSenseClass::Connected()
{
    if (!client)
        return false;

    return client->isConnected();
}


//PRINT RAW DATA
void WolfSenseClass::PrintRawData()
{
    if (!newPacket)
        return;

    newPacket = false;

    Serial.print("DATA: ");


    for (int i = 0; i < packetLen; i++)
    {
        
        // NORMALIZED JOYSTICK
        if (i == 0)
        {
            if (JoyL_X >= 0)
                Serial.print("+");

            Serial.print(JoyL_X);
        }

        else if (i == 1)
        {
            if (JoyL_Y >= 0)
                Serial.print("+");

            Serial.print(JoyL_Y);
        }

        else if (i == 2)
        {
            if (JoyR_X >= 0)
                Serial.print("+");

            Serial.print(JoyR_X);
        }

        else if (i == 3)
        {
            if (JoyR_Y >= 0)
                Serial.print("+");

            Serial.print(JoyR_Y);
        }


        // RAW BYTE
        else
        {
            if (packetData[i] < 100)
                Serial.print("0");

            if (packetData[i] < 10)
                Serial.print("0");

            Serial.print(packetData[i]);
        }

        Serial.print(" ");
    }

    // BUTTON PRINT
    Serial.print(" | BTN: ");

    bool hasButton = false;

    // DPAD
    if (UP) {
        Serial.print("UP ");
        hasButton = true;
    }

    if (DOWN) {
        Serial.print("DOWN ");
        hasButton = true;
    }

    if (LEFT) {
        Serial.print("LEFT ");
        hasButton = true;
    }

    if (RIGHT) {
        Serial.print("RIGHT ");
        hasButton = true;
    }


    // ABXY
    if (A) {
        Serial.print("A ");
        hasButton = true;
    }

    if (B) {
        Serial.print("B ");
        hasButton = true;
    }

    if (X) {
        Serial.print("X ");
        hasButton = true;
    }

    if (Y) {
        Serial.print("Y ");
        hasButton = true;
    }


    // SHOULDER BUTTONS
    if (LB) {
        Serial.print("LB ");
        hasButton = true;
    }

    if (RB) {
        Serial.print("RB ");
        hasButton = true;
    }

    if (LT) {
        Serial.print("LT ");
        hasButton = true;
    }

    if (RT) {
        Serial.print("RT ");
        hasButton = true;
    }

    // SYSTEM BUTTONS
    if (START) {
        Serial.print("START ");
        hasButton = true;
    }

    if (SELECT) {
        Serial.print("SELECT ");
        hasButton = true;
    }

    if (L3) {
        Serial.print("L3 ");
        hasButton = true;
    }

    if (R3) {
        Serial.print("R3 ");
        hasButton = true;
    }

    // MACRO BUTTONS
    if (M1) {
        Serial.print("M1 ");
        hasButton = true;
    }

    if (M2) {
        Serial.print("M2 ");
        hasButton = true;
    }

    if (FN) {
        Serial.print("FN ");
        hasButton = true;
    }

    if (POWER) {
        Serial.print("POWER ");
        hasButton = true;
    }

    if (!hasButton) {
        Serial.print("NONE");
    }

    Serial.println();
}
