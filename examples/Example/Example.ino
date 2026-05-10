//  MAKE BY TONY HUYNH TRAN
//  IN 5/2026
//  HAVE A GOOD DAY MY FRIENDS

#include <WolfSense.h>

void setup()
{   
    Serial.begin(2000000);                  // High-speed baud rate for real-time raw data streaming, high-quality USB cable recommended
    WolfSense.SetMAC("A4:C1:38:37:60:E0");   // Replace with your gamepad's MAC address
    WolfSense.SetDeathZone(20);
    WolfSense.begin();
}

void loop()
{
    WolfSense.loop();
    if (WolfSense.Connected()) 
    {
        // READ ANALOG
        Serial.print("LX: ");
        Serial.print(WolfSense.JoyL_X);

        Serial.print(" | LY: ");
        Serial.print(WolfSense.JoyL_Y);

        Serial.print(" | RX: ");
        Serial.print(WolfSense.JoyR_X);

        Serial.print(" | RY: ");
        Serial.print(WolfSense.JoyR_Y);


        // READ BUTTONS 
        Serial.print(" | BTN: ");
        bool hasButton = false;

            // DPAD
            if (WolfSense.UP) {
                Serial.print("UP ");
                hasButton = true;
            }

            if (WolfSense.DOWN) {
                Serial.print("DOWN ");
                hasButton = true;
            }

            if (WolfSense.LEFT) {
                Serial.print("LEFT ");
                hasButton = true;
            }

            if (WolfSense.RIGHT) {
                Serial.print("RIGHT ");
                hasButton = true;
            }


            // ABXY
            if (WolfSense.A) {
                Serial.print("A ");
                hasButton = true;
            }

            if (WolfSense.B) {
                Serial.print("B ");
                hasButton = true;
            }

            if (WolfSense.X) {
                Serial.print("X ");
                hasButton = true;
            }

            if (WolfSense.Y) {
                Serial.print("Y ");
                hasButton = true;
            }


            // SHOULDER BUTTONS
            if (WolfSense.LB) {
                Serial.print("LB ");
                hasButton = true;
            }

            if (WolfSense.RB) {
                Serial.print("RB ");
                hasButton = true;
            }

            if (WolfSense.LT) {
                Serial.print("LT ");
                hasButton = true;
            }

            if (WolfSense.RT) {
                Serial.print("RT ");
                hasButton = true;
            }


            // SYSTEM BUTTONS
            if (WolfSense.START) {
                Serial.print("START ");
                hasButton = true;
            }

            if (WolfSense.SELECT) {
                Serial.print("SELECT ");
                hasButton = true;
            }

            if (WolfSense.L3) {
                Serial.print("L3 ");
                hasButton = true;
            }

            if (WolfSense.R3) {
                Serial.print("R3 ");
                hasButton = true;
            }


            // MACRO BUTTONS
            if (WolfSense.M1) {
                Serial.print("M1 ");
                hasButton = true;
            }

            if (WolfSense.M2) {
                Serial.print("M2 ");
                hasButton = true;
            }

            if (WolfSense.FN) {
                Serial.print("FN ");
                hasButton = true;
            }

            if (WolfSense.POWER) {
                Serial.print("POWER ");
                hasButton = true;
            }


            // NONE BUTTONS ACTIVATE
            if (!hasButton)
            {
                Serial.print("NONE");
            }
    }
    else
    {
        Serial.println();
        Serial.print("DISCONNECT");
    }
        Serial.println();
        delay(10);
}
