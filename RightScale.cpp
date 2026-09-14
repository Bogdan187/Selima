#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "RightScale.h"

void konfiguration_waage_rechts()
{
  while(konfiguration_waage_rechts_fertig==false)
  {
    while(digitalRead(taster_enter)==HIGH)                            //Länge der Kette
    {
      delay(200);
      referenzfahrt_done=false;
      lcd.setCursor(0, 0);
      lcd.print("Wie Hoch ist Waage");
      lcd.setCursor(0, 1);
      lcd.print("Hoehe:  "); lcd.print(hoehe_waage_rechts);
      lcd.print(" mm   ");
      lcd.setCursor(0, 2);
      lcd.print("-: -1          +: +1");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT     ");
      if (digitalRead(taster_plus) == LOW) 
      { 
       hoehe_waage_rechts++;
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW) 
      { 
        if (hoehe_waage_rechts> 50)                                 // Mindesthöhe
        {
          hoehe_waage_rechts--;
          delay(20); 
        }
            
      }
    }
    lcd.clear();
    delay(500);

    while(digitalRead(taster_enter)==HIGH)              //Abstand Kette-Boden 
    {
      lcd.setCursor(0,0);
      lcd.println("   Boden - Kette  ");
      lcd.setCursor(0, 1);
      lcd.print("Abstand:  "); lcd.print(abstand_kette_boden);
      lcd.print(" mm   ");
      lcd.setCursor(0, 2);
      lcd.print("-: -2          +: +2");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT     ");
      if (digitalRead(taster_plus) == LOW) 
      { 
       abstand_kette_boden+= 2;
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW) 
      { 
        abstand_kette_boden-=2;
        delay(20); 
      }
    }
    lcd.clear();
    delay(500);


    while(digitalRead(taster_enter)==HIGH)                  //Abstand der Gewichtsgliedern
    {
      lcd.setCursor(0,0);
      lcd.print("Abstand Gewichte:");
      lcd.setCursor(0, 1);
      lcd.print("Breite:   "); lcd.print(gap_scheiben_rechts);
      lcd.print(" mm ");
      lcd.setCursor(0, 2);
      lcd.print("-: -0,05    +: +0,05");
      lcd.setCursor(0, 3);
      lcd.print("    Enter: ENT     ");
      if (digitalRead(taster_plus) == LOW) 
      { 
        gap_scheiben_rechts+= 0.05; 
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW) 
      {
        gap_scheiben_rechts-=0.05;
        delay(20); 
      }
    }
    lcd.clear();
    delay(500);

    while(digitalRead(taster_enter)==HIGH)              //Anzahl der Gewichte
    {
      lcd.setCursor(0,0);
      lcd.print("Anzahl Gewichte:");
      lcd.setCursor(0, 1);
      lcd.print("Anzahl:  "); lcd.print(anzahl_scheiben_rechts);
      lcd.print(" Scheiben");
      lcd.setCursor(0, 2);
      lcd.print("-: -1          +: +1");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT      ");
      if (digitalRead(taster_plus) == LOW) 
      {
        anzahl_scheiben_rechts++; 
        delay(80); 
      }
      if (digitalRead(taster_minus) == LOW) 
      {
        if(anzahl_scheiben_rechts>1)
        {
          anzahl_scheiben_rechts--;
          delay(80); 
        }
        else
        {
          anzahl_scheiben_rechts=1;
        }
      }
    }
    lcd.clear();
    delay(500);
    

    while(digitalRead(taster_enter)==HIGH)                              //X Position der Waage rechts
    {
      lcd.setCursor(0,0);
      lcd.print("Wo ist die Waage?");
      lcd.setCursor(0,1);
      lcd.print("Position X:  "); lcd.print(position_waage_rechts);
      lcd.print(" cm");
      lcd.setCursor(0, 2);
      lcd.print("-: -1        +: +1");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT      ");
      if (digitalRead(taster_plus) == LOW) 
      {
        position_waage_rechts++; 
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW)
      {
        if(position_waage_rechts>=1)
        {
          position_waage_rechts--;
          delay(20); 
        }
        else
        {
          position_waage_rechts=1;
        }
      }
    }
    lcd.clear();
    konfiguration_waage_rechts_fertig=true;
    delay(500);
  }
      
  lcd.setCursor(0,0);
  lcd.print("Fahre zur Waage...");
  
  z_verfahren_position(speedMotorZ,0);        //0

  position_x_waage_rechts_koordinate= position_waage_rechts*SCHRITT_X*10;
  x_verfahren_position(speedMotorX,position_x_waage_rechts_koordinate);   //Waage x achse

  position_z_waage_rechts_koordinate= (abstand_kette_boden*SCHRITT_Z)-(hoehe_waage_rechts*SCHRITT_Z);        //Koordinate des Offsets berechnen
  z_verfahren_position(speedMotorZ, position_z_waage_rechts_koordinate);

  lcd.clear();
  gewichte_auflegen_rechts();
}

void gewichte_auflegen_rechts()
{
  while(digitalRead(taster_xn)==HIGH)
  {
    while(digitalRead(taster_enter)==HIGH)
    {
      lcd.setCursor(0,0);
      lcd.print("Wie viele Scheiben");
      lcd.setCursor(0,1);
      lcd.print("sollen drauf?: ");lcd.print(gewichte_drauf_rechts);
      lcd.setCursor(0, 2);
      lcd.print("-: -1        +: +1");
      lcd.setCursor(0, 3);
      lcd.print("Enter: ENT/Abbruch:<-");
        if (digitalRead(taster_plus) == LOW) 
        {
          if(gewichte_drauf_rechts <  anzahl_scheiben_rechts)
          {
            gewichte_drauf_rechts++; 
            delay(200); 
          }
          else
          {
            gewichte_drauf_rechts=anzahl_scheiben_rechts;                 //Maximum von Scheiben 
          }
        }
        if (digitalRead(taster_minus) == LOW) 
        {
          if(gewichte_drauf_rechts >= 1)
          {
            gewichte_drauf_rechts--; 
            delay(200); 
          }
          else
          {
            gewichte_drauf_rechts=0;
          }
        }
    }
    long gewichte_ablegen_rechts=position_z_waage_rechts_koordinate+(gap_scheiben_rechts*gewichte_drauf_rechts*SCHRITT_Z);
    z_verfahren_position(speedGewichteAblegen,gewichte_ablegen_rechts);
  }
  z_verfahren_position(speedMotorZ, 0);
  lcd.clear();
  gewichte_auflegen_rechts_exit();
}

void gewichte_auflegen_rechts_exit()
{ 
  while(digitalRead(taster_xn)==HIGH)
  {
    lcd.setCursor(0,0);
    lcd.print("<-: Konfiguration neu");
    lcd.setCursor(0,1);
    lcd.print("->: Manueller Mod.");
    lcd.setCursor(0,2);
    lcd.print("+: Wieder runter");
      if(digitalRead(taster_xp)==LOW)
      {
        lcd.clear();
        manuellerModus=true;
        break;
      }
      if(digitalRead(taster_plus)==LOW)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Fahre wieder");
        lcd.setCursor(0,1);
        lcd.print("runter");
        z_verfahren_position(speedMotorZ, position_z_waage_rechts_koordinate);         //fahre wieder zun Offset
        lcd.clear();
        gewichte_auflegen_rechts();
        break;
      }
  }
  lcd.clear();
  referenzfahrt=true;
  referenzfahrt_anzeige_0=true;
}
