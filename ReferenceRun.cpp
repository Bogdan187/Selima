#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "ReferenceRun.h"


void processReferenceRun()
{
    // -------------------- Referenzfahrt --------------------
    if ((referenzfahrt_anzeige_0) == true && (referenzfahrt) == true)                 //Standart am Anfang
    {
      lcd.setCursor(0,0);
      lcd.print("Referenzfahrt");
      lcd.setCursor(0,1);
      lcd.print("bereit!");
      lcd.setCursor(0,2);
      lcd.print("Bitte ENT");
      lcd.setCursor(0,3);
      lcd.print("Taste halten!"); 
      Serial.println("Willkommen. Warten auf Eingabe. Für Referenzfahrt = ENT");                                                                 //Output
    }                
    //Startbedingungen prüfen
    if (referenzfahrt == true && ((digitalRead(taster_enter) == LOW) || taster_enter_simuliert==true))
    {
      if (digitalRead(taster_enter)==LOW || taster_enter_simuliert==true)
      {
        Serial.println("Referenzfahrt in gange");
        lcd.clear();                                                //Start von der Referenzfahrt
        lcd.setCursor(0,0);
        lcd.print("Referenzfahrt");
        lcd.setCursor(0,1);
        lcd.print("beginnt");
        taster_enter_simuliert=false;
        referenzfahrt_anzeige_0=false;
        referenzfahrt_z = true;
        referenzfahrt = false;
      }
      else if(digitalRead(taster_plus)==LOW || taster_plus_simuliert==true)               //Die Referenzfahrt überspringen und Achsen manuell verfahren
      {
        referenzfahrt_anzeige_0=false;
        referenzfahrt= false;
        manuellerModus= true;
      }
    }
  
   if (referenzfahrt_z == true)                 //Fange referenzfahrt an mit Z Achse
    {
      Serial.println("Referenzfahrt in Richtung Z+ ");
      switch(z_phase)
      {
          case 0:                                                 // Nach oben fahren
              if (digitalRead(endschalter_zp) == LOW)             //Solange der Endschalter ZP nicht betätigt wird
              {
                lcd.setCursor(0, 3);
                lcd.print("Fahre in Richtung Z+");
                z_verfahren(plus, speedMotorReferenzfahrt_Z);  
              }
              else
              {
                  z_phase = 1;
                  Serial.println("Referenzfahrt in Richtung Z- bis den Endschalter losgelassen wird");
              }
          break;

          case 1:
              lcd.setCursor(0, 1);
              lcd.print("                   ");
              lcd.setCursor(0, 3);
              lcd.print("Fahre in Richtung Z-");
            

              if (digitalRead(endschalter_zp) == HIGH)    // solange noch gedrückt
              {
                z_verfahren(minus, speedMotorReferenzfahrtNegativ_Z);
              }
              else                                       // Schalter losgelassen
              {
              z_phase = 2;
              Serial.println("Referenzfahrt auf der Achse Z wurde abgeschlossen.");
              }
              break;

          case 2:                                                 // Fertig mit Z Achse
              digitalWrite(motorbremse, LOW);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Referenz Z-Achse");
              lcd.setCursor(0,1);
              lcd.print("abgeschlossen");
              if(x_referenzfahrt_skip==true)
              {
                referenzfahrt_z = false;
                ist_position_z= 0.0;
                referenzfahrt_z_done=true;
                z_phase = 0;            
                delay(1000);
              }
              else 
              {
              referenzpunkt_z = true;
              referenzfahrt_z = false;
              referenzfahrt_x = true;
              ist_position_z= 0.0;
              z_phase = 0;            
              delay(1000);
              }
            

            case 3:

          break;
      }
    }
    if (referenzfahrt_x == true)
    {
      Serial.println("Referenzfahrt in Richtung X-");
      switch (x_phase)
      {
        case 0:   // Nach oben fahren
        if (digitalRead(endschalter_xn) == LOW)             //Solange der Endschalter ZP nicht betätigt wird
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Referenzfahrt");
            lcd.setCursor(0, 3);
            lcd.print("Fahre in Richtung X-");
            x_verfahren(minus, speedMotorReferenzfahrtNegativ_X);  
          }
        else
          {
            x_phase = 1;
            Serial.println("Referenzfahrt in X+");
          }
        break;

        case 1:
         if (digitalRead(endschalter_xn)== HIGH)                        //Der Endschalter XN wurde betätigt
          {
            lcd.setCursor(0, 1);
            lcd.print("                   ");
            lcd.setCursor(0, 3);
            lcd.print("Fahre in Richtung X+");
            x_phase = 2;
            x_verfahren(plus, speedMotorReferenzfahrt_X);             //Fahren in Richtung X+ um einen Referenzpunkt auf der Z Achse zu holen. 
          }
        break;

        case 2:
          taster_enter_simuliert=false;
          digitalWrite(motorbremse, LOW);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Referenzfahrt");
          lcd.setCursor(0,1);
          lcd.print("abgeschlossen");
          Serial3.print("R+\r");
          delay(3000);
          lcd.clear();
          ist_position_x=0;
          referenzfahrt_x = false;
          x_phase = 0;
          referenzfahrt_anzeige_fertig = true;
          referenzfahrt_schnittstelle=true;
          break;
      }
    }

    if (referenzfahrt_anzeige_fertig == true)
    {
      referenzfahrt_anzeige_0 = false;
      referenzfahrt_anzeige_fertig = false; 
      referenzfahrt_done= true;                                       //Sprung zur Auswahl Waagen
      Offsetz = false;                                                // Offset wieder erlauben
      if (ReferenzSchnittstelle == true)
      {
        delay(1);
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        ReferenzSchnittstelle = false;
        inputString = "";
      }
    }
}

void referenzfahrt_z_vor_x()
{
  switch(z_phase_referenzfahrt)
  {
    case 0:   // Z nach oben bis Endschalter ZP
    {
      if (digitalRead(endschalter_zp) == LOW)
      {
        // Nur während z_verfahren() Referenzmodus aktivieren
        referenzfahrt_z = true;

        z_verfahren(plus, speedMotorReferenzfahrt_Z);

        referenzfahrt_z = false;
      }

      // z_verfahren() kommt erst zurück, wenn ZP betätigt wurde
      if (digitalRead(endschalter_zp) == HIGH)
      {
        z_phase_referenzfahrt = 1;
      }

      break;
    }


    case 1:   // Wieder herunter, bis Endschalter ZP frei ist
    {
      if (digitalRead(endschalter_zp) == HIGH)
      {
        // Nur während z_verfahren() Referenzmodus aktivieren
        referenzfahrt_z = true;

        z_verfahren(minus, speedMotorReferenzfahrtNegativ_Z);

        referenzfahrt_z = false;
      }

      // z_verfahren() kommt erst zurück, wenn ZP wieder frei ist
      if (digitalRead(endschalter_zp) == LOW)
      {
        ist_position_z = 0;

        // 2 = Z-Referenzierung vollständig fertig
        z_phase_referenzfahrt = 2;
      }

      break;
    }


    case 2:
    {
      // Fertig -> X darf jetzt fahren
      break;
    }
  }
}