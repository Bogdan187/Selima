#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "endschalterTest.h"

void endschalterTest()
{
    // Der Test soll nur einmal nach dem Einschalten laufen.
    static bool testAbgeschlossen = false;

    if (testAbgeschlossen)
    {
        return;
    }

    // Während des Tests dürfen die Achsen mit den Tasten bewegt werden.
    bool vorherigerManuellerModus = manuellerModus;
    manuellerModus = true;

    // Diese Variable darf nicht true sein, da die Motor-Schleifen
    // sonst die Tasten und Endschalter ignorieren würden.
    endschaltertest_verfahren = false;

    unsigned long alleFreiSeit = 0;

    lcd.clear();

    while (!testAbgeschlossen)
    {
        int zustandZP = digitalRead(endschalter_zp);
        int zustandZN = digitalRead(endschalter_zn);
        int zustandXP = digitalRead(endschalter_xp);
        int zustandXN = digitalRead(endschalter_xn);

        // Aktuelle Zustände anzeigen
        lcd.setCursor(0, 0);
        lcd.print("EZP:");
        lcd.print(zustandZP);
        lcd.print("    EZN:");
        lcd.print(zustandZN);
        lcd.print("  ");

        lcd.setCursor(0, 1);
        lcd.print("EXP:");
        lcd.print(zustandXP);
        lcd.print("    EXN:");
        lcd.print(zustandXN);
        lcd.print("  ");

        // Bei deiner Maschine gilt:
        // LOW  = frei
        // HIGH = betätigt
        bool alleEndschalterFrei =
            zustandZP == LOW &&
            zustandZN == LOW &&
            zustandXP == LOW &&
            zustandXN == LOW;

        if (alleEndschalterFrei)
        {
            lcd.setCursor(0, 2);
            lcd.print("Alle Schalter frei ");
            lcd.setCursor(0, 3);
            lcd.print("Test abgeschlossen ");

            // Alle Endschalter müssen eine Sekunde lang frei bleiben.
            if (alleFreiSeit == 0)
            {
                alleFreiSeit = millis();
            }

            if (millis() - alleFreiSeit >= 1000UL)
            {
                testAbgeschlossen = true;
            }
        }
        else
        {
            alleFreiSeit = 0;

            lcd.setCursor(0, 2);
            lcd.print("Endschalter aktiv! ");
            lcd.setCursor(0, 3);
            lcd.print("Achse freifahren   ");

            // Manuelles Freifahren der Achsen
            if (digitalRead(taster_xn) == LOW)
            {
                x_verfahren(minus, speedMotorX);
            }
            else if (digitalRead(taster_xp) == LOW)
            {
                x_verfahren(plus, speedMotorX);
            }
            else if (digitalRead(taster_zn) == LOW)
            {
                z_verfahren(minus, speedMotorZ);
            }
            else if (digitalRead(taster_zp) == LOW)
            {
                z_verfahren(plus, speedMotorZ);
            }
        }

        delay(20);
    }

    manuellerModus = vorherigerManuellerModus;
    endschaltertest_verfahren = false;

    lcd.clear();
    Serial.println("Endschaltertest abgeschlossen.");
}