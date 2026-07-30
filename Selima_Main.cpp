// -------------------- Initialisierung --------------------
// 20.11.23 H.Witte: Serielle Schnittstelle hinzu, Schritte angepasst
// 18.02.26 B.Saeisih: Umschreibung des Codes für die 2-Achs-Celima-Maschine
// //
// 29.07.2026 B. Saeisih Gemeinsame Codebasis für Arduino IDE und CLion/PlatformIO.

#include <Arduino.h>
#include <Wire.h>
#include "endschalterTest.h"
#include "Config.h"
#include "Globals.h"
#include "ReferenceRun.h"
#include "MenuControl.h"
#include "Dauerbelastung.h"
#include "LeftScales.h"
#include "SerialInterface.h"
#include "MotorControl.h"

void setup()
{
  // -------------------- Tastenfeld --------------------
  pinMode(taster_xn,    INPUT_PULLUP);
  pinMode(taster_xp,    INPUT_PULLUP);
  pinMode(taster_enter, INPUT_PULLUP);
  pinMode(taster_0,     INPUT_PULLUP);
  pinMode(taster_zn,    INPUT_PULLUP);
  pinMode(taster_zp,    INPUT_PULLUP);
  pinMode(taster_plus,  INPUT_PULLUP);
  pinMode(taster_minus, INPUT_PULLUP);
  // ----------------------------------------

  // -------------------- Endschalter --------------------
  pinMode(endschalter_xp, INPUT);                       //High wenn betätigt wird
  pinMode(endschalter_xn, INPUT);
  pinMode(endschalter_zp, INPUT_PULLUP);                //PULLUP damit auch High wenn betätigt wird.
  pinMode(endschalter_zn, INPUT_PULLUP);
  // ----------------------------------------

  // -------------------- Motorsteuerkarte x-Achse --------------------
  pinMode(dir_x,  OUTPUT);
  pinMode(pul_x,  OUTPUT);
  pinMode(en_x,   OUTPUT);
  // ----------------------------------------

  // -------------------- Motorsteuerkarte z-Achse --------------------
  pinMode(dir_z,  OUTPUT);
  pinMode(pul_z,  OUTPUT);
  pinMode(en_z,   OUTPUT);
  // ----------------------------------------


  // -------------------- Motorbremse --------------------
  pinMode(motorbremse, OUTPUT);
  // ----------------------------------------

  //---------------------- Spannungsversorgung von Wägezellen-----------
  pinMode(spannungsversorgung24, OUTPUT);
  pinMode(spannungsversorgung12, OUTPUT);
  digitalWrite(spannungsversorgung24,LOW);        //Soll beim einschalten afu jeden fall aus sein
  digitalWrite(spannungsversorgung12, LOW);
  //--------------------------------------------------------------------

  //---------------------- Steuerung von Rollos------------------------
  pinMode(rollo_up, OUTPUT);
  pinMode(rollo_down, OUTPUT);
  digitalWrite(rollo_down, LOW);
  digitalWrite(rollo_up,LOW);
  //--------------------------------------------------------------------

  //  Serielle Schnittstelle

  lcd.init();
  lcd.backlight();

  Serial3.begin(9600, SERIAL_8N1);
    // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  // -------------------- LCD-Display (20x4) --------------------
  Serial.begin(9600);
  // ----------------------------------------
}

void loop()
{
  endschalterTest();
  processReferenceRun();
  processMainMenuAndManualMode();
  processDauerbelastung();
  processLeftScaleRun();
  processSerialCommands();
  processZBrake();
}
