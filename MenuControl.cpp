#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "LeftScales.h"
#include "RightScale.h"
#include "MenuControl.h"

void processMainMenuAndManualMode()
{
    //--------------------------- Koordinaten Anzeigen wenn über Schnittstelle die Befehle ankommen ----------------

    if(serielle_schnittstelle==true && digitalRead(taster_enter)==HIGH)
    {
        lcd.setCursor(0, 0);
        lcd.print("Aktuelle Position:");

        lcd.setCursor(0, 1);
        lcd.print("X:                 ");  // alte Werte entfernen
        lcd.setCursor(3, 1);
        lcd.print(ist_position_x / SCHRITT_X);
        lcd.print(" mm");

        lcd.setCursor(0, 2);
        lcd.print("Z:                 ");  // alte Werte entfernen
        lcd.setCursor(3, 2);
        lcd.print(ist_position_z / SCHRITT_Z);
        lcd.print(" mm");

        lcd.setCursor(0, 3);
        lcd.print("   ENT: Verlassen  ");
    }


    //----------------------Auswahl Waagen, Manueller Modus, Custom Mode-------------------------
    else
    {
      referenzfahrt_schnittstelle=false;
      if (referenzfahrt_done==true)
      {
        lcd.setCursor(0,0);
        lcd.print("Referenzfahrt fertig");
        lcd.setCursor(0,1);
        lcd.print("<- : Manueller Modus");
        if(digitalRead(taster_xn)==LOW)
        {
          delay(200);
          lcd.clear();
          manuellerModus=true;
          referenzfahrt_done=false;
        }
      
        lcd.setCursor(0,2);
        lcd.print("-> : Custom Mode");
        if(digitalRead(taster_xp)==LOW)
        {
          lcd.clear();        
          konfiguration_waage_rechts_fertig=false;
          konfiguration_waage_rechts();
        }                              

        lcd.setCursor(0,3);
        lcd.print(" - : Auswahl Waagen"); 

      
      }
    
    
      if(digitalRead(taster_xp)==LOW)
      {
        while(digitalRead(taster_enter)==HIGH)
        {
          delay(200);
          referenzfahrt_done=false;
          lcd.setCursor(0, 0);
          lcd.print("--Hoehe einstellen--");
          lcd.setCursor(0, 1);
          lcd.print("Hoehe:  "); lcd.print(hoehe_waage_rechts);
          lcd.print(" mm ");
          lcd.setCursor(0, 2);
          lcd.print("-: -5          +: +5");
          lcd.setCursor(0, 3);
          lcd.print("     Enter: ENT");
          if (digitalRead(taster_plus) == LOW) 
          { 
            hoehe_waage_rechts+= 5; 
            delay(100); 
          }
          if (digitalRead(taster_minus) == LOW) 
          { 
            if (hoehe_waage_rechts> 50)                                 // Mindesthöhe
            {
              hoehe_waage_rechts-=5; 
            }
            delay(100); 
          }
        }
        lcd.setCursor(0,0);
        lcd.println("Fahre in rechten");
        lcd.setCursor(4,1);
        lcd.println("Bereich");
        lcd.setCursor(0,2);
        lcd.print("      ------->     ");
        delay(3000);
        long schritte_waage_rechts =grenze_z - (hoehe_waage_rechts *160);
        x_verfahren_position(speedMotorX, x_koordinate_waage_rechts);
        z_verfahren_position(speedMotorZ, schritte_waage_rechts);
        lcd.clear();
        manuellerModus=true;
      }
      if(digitalRead(taster_minus)==LOW)                                                     //eingang in Auswahl Waagen
      {
        delay(200);
        lcd.clear();
        referenzfahrt_done=false;
        auswahlWaagen();
      }
    }

    // -------------------- Manuell verfahren --------------------

   if ((referenzfahrt==true && digitalRead(taster_plus) == LOW) || manuellerModus ==true)
   {
      lcd.setCursor(0,0);
      lcd.print("Willkommmen in");
      lcd.setCursor(0,1);
      lcd.print("Manuellen Modus");
      lcd.setCursor(0,2);
      lcd.print("zum verlassen = ENT");
      lcd.setCursor(0,3);
      lcd.print("                 ");
      referenzfahrt = false;
      referenzfahrt_anzeige_0= false;
      manuellerModus= true;
      auswahlWaagen_status= false;
      //Serial.println("Willkommen in manuellen Modus. Zum verlassen die Taste ENT drücken");
      if (digitalRead(taster_enter)==LOW && referenzfahrt_done==false)
      {
        lcd.clear();
        referenzfahrt=true;
        manuellerModus=false;
      }
      if ((digitalRead(taster_xp) == LOW) || (digitalRead(taster_xn) == LOW) || (digitalRead(taster_zp) == LOW) || (digitalRead(taster_zn) == LOW) || (digitalRead(taster_enter)==LOW) || (digitalRead(taster_0)==LOW))
      {
        lcd.clear();
        if ((digitalRead(taster_xp) == LOW && digitalRead(endschalter_xp)==LOW))
        {
          lcd.setCursor(0,3);
          lcd.print("X Position:");
          lcd.print(ist_position_x/SCHRITT_X);
          lcd.print("mm");
          x_verfahren(plus, speedMotorX);
        }
        if ((digitalRead(taster_xn) == LOW) && digitalRead(endschalter_xn)==LOW)
        {
          lcd.setCursor(0,3);
          lcd.print("X Position");
          lcd.print(ist_position_x/SCHRITT_X);
          lcd.print("mm");
          x_verfahren(minus, speedMotorX);
        }
        if ((digitalRead(taster_zp) == LOW) && digitalRead(endschalter_zp)==LOW)
        {
          lcd.setCursor(0,3);
          lcd.print("Z Position");
          lcd.print(ist_position_z/SCHRITT_Z);
          lcd.print("mm");
          z_verfahren(plus, speedMotorZ);

        }
        if ((digitalRead(taster_zn) == LOW) && digitalRead(endschalter_zn)==LOW)
        {
          lcd.setCursor(0,3);
          lcd.print("Z Position");
          lcd.print(ist_position_z/SCHRITT_Z);
          lcd.print("mm");
          z_verfahren(minus, speedMotorZ);
        }
        if(digitalRead(taster_enter)== LOW)
        {
          lcd.clear();
          manuellerModus=false;
          referenzfahrt=true;
          referenzfahrt_anzeige_0=true;
        }
        if(digitalRead(taster_0)==LOW)
        {
          lcd.clear();
          nullpunkt_z();
        }   
      }
    }
}
