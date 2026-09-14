#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "Dauerbelastung.h"

void gewichteVerfahren_dauerbelastung()
{
  Serial.println("Willkommen in den Modus für die Waage die rechts steht.");
  Serial.println("Zum Verlassen und Referenzfahrt machen, bitte den Taster mit dem Pfeil nach oben drücken");
  while(digitalRead(taster_zp)==HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("Wie viel Last soll");
    lcd.setCursor(0,1);
    lcd.print("aufgelegt werden?");
    lcd.setCursor(8,2);
    lcd.print(gewichtOptionen[gewichtOptionen_auswahl]);
    lcd.print("kg");
    lcd.setCursor(0,3);
    lcd.print("-       ENT       +");

    if(digitalRead(taster_plus)==LOW)
    {
      gewichtOptionen_auswahl++;

      if(gewichtOptionen_auswahl >= 6)   // Array hat 6 Elemente
      {
        gewichtOptionen_auswahl = 0;     // wieder von vorne
      }
    }
    if(digitalRead(taster_minus)==LOW)
    {
      gewichtOptionen_auswahl--;

      if(gewichtOptionen_auswahl < 0)   // Array hat 6 Elemente
      {
        gewichtOptionen_auswahl = 6;     // wieder von hinten
      }
    }

      lcd.setCursor(8,2);
      lcd.print("          ");   // alte Zahl löschen
      lcd.setCursor(8,2);
      lcd.print(gewichtOptionen[gewichtOptionen_auswahl]);
      lcd.print("kg");

      delay(100);           //  Entprellen

    if(digitalRead(taster_enter)==LOW)
    {
      delay(100);
      switch (gewichtOptionen_auswahl)
      {
        case 0:
          z_verfahren_position(speedGewichteAblegen, position_stufe_a);
          break;
        case 1:
          z_verfahren_position(speedGewichteAblegen, position_stufe_b);
          break;
        case 2:
          z_verfahren_position(speedGewichteAblegen, position_stufe_c);
          break;
        case 3:
          z_verfahren_position(speedGewichteAblegen, position_stufe_d);
          break;
        case 4:
          z_verfahren_position(speedGewichteAblegen, position_stufe_e);
          break;
        case 5:
          z_verfahren_position(speedGewichteAblegen, position_stufe_f);
          break;
        case 6:
          z_verfahren_position(speedGewichteAblegen,ist_nullpunkt_z);
          break;
      }
    }
  }
  while(nullpunkt_gesetzt==true)
  {
    lcd.setCursor(0,0);
    lcd.print("Dauerbelastungstest");
    lcd.setCursor(0,1);
    lcd.print("Staten?");
    lcd.setCursor(0,2);
    lcd.print("Ja= nach unten Nein= 0");
    if (digitalRead(taster_zn)==LOW)
    {
      lcd.clear();
      delay(5000);
      dauerbelastungstest=true;
      nullpunkt_gesetzt=false;
    }
    if(digitalRead(taster_0)==LOW)
    {
      lcd.clear();
      delay(5000);
      gewichteVerfahren_dauerbelastung();
    }
  }
}

void processDauerbelastung()
{
    if(dauerbelastungstest==true)
    {
      for(int anzahl_zyklen=0;anzahl_zyklen<100; anzahl_zyklen++)
      {
      lcd.setCursor(0,0);
      lcd.print("Anzahl Zyklen: ");
      lcd.print(anzahl_zyklen);

      z_verfahren_position(speedGewichteAblegen, position_stufe_a);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen, position_stufe_b);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen, position_stufe_c);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen, position_stufe_d);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen, position_stufe_e);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen, position_stufe_f);
      delay(2000);

      z_verfahren_position(speedGewichteAblegen,ist_nullpunkt_z);
      delay(2000);

      } 
      dauerbelastungstest=false;
    }
}
