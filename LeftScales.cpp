#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "LeftScales.h"

void auswahlWaagen()
{
  waagennummer = 1;
  bool setupWiederholen = true;

  while(setupWiederholen)
  {
    setupWiederholen = false;

    for(int i = 0; i < 12; i++)
    {
      waagennummer = i + 1;

      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Setup Waage:   ");
      lcd.print(waagennummer);

      lcd.setCursor(0,1);
      lcd.print("Aktivieren?");

      lcd.setCursor(0,3);
      lcd.print("+ : Ja  Ent: Nein");

      while(true)
      {
        if(digitalRead(taster_plus) == LOW)
        {
          delay(200);
          eigenschaftenWaage(waagennummer);
          break;
        }

        if(digitalRead(taster_enter) == LOW)
        {
          delay(200);

          lcd.clear();
          lcd.setCursor(5,1);
          lcd.print("deaktiviert");

          waage_deaktivieren(waagennummer);

          delay(1500);
          break;
        }
      }
    }

    bool eineAktiv = false;

    for(int i=0;i<12;i++)
    {
      if(waageHight[i] != 0)
      {
        eineAktiv = true;
        break;
      }
    }

    if(!eineAktiv)
    {
      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Es wurde keine");

      lcd.setCursor(0,1);
      lcd.print("Waage ausgewaehlt");

      lcd.setCursor(0,2);
      lcd.print("Set-Up wiederholen?");

      lcd.setCursor(0,3);
      lcd.print("+:Ja    Man.Mod: -");

      while(true)
      {
        if(digitalRead(taster_plus) == LOW)
        {
          delay(300);
          setupWiederholen = true;
          break;
        }

        if(digitalRead(taster_minus) == LOW)
        {
          manuellerModus = true;
          delay(300);
          break;
        }
      }
    }
  }

  lcd.clear();
  lcd.print("Setup beendet!");
  delay(1000);

  Serial.print("Hoehe ");
  for (int i = 0; i < 12; i++)
  {
    Serial.print(waageHight[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Last ");
  for (int i = 0; i < 12; i++)
  {
    Serial.print(maxLast[i]);
    Serial.print(" ");
  }

  lcd.clear();
  konfiguration_waagen = true;
}

void eigenschaftenWaage(int nummer)
{
  int phase = 0;
  bool fertig = false;

  int index = nummer - 1;

  lcd.clear();

  while(!fertig)
  {
    if(phase == 0)
    {
      lcd.setCursor(0,0);
      lcd.print("Waage: ");
      lcd.print(nummer);
      lcd.print(": Hoehe");

      lcd.setCursor(0,1);
      lcd.print("Hoehe: ");
      lcd.print(waageHight[index]);
      lcd.print("mm   ");

      lcd.setCursor(0,2);
      lcd.print("-5                +5");

      lcd.setCursor(0,3);
      lcd.print("  ENT: Weiter  ");

      if(digitalRead(taster_plus) == LOW)
      {
        waageHight[index] += 5;
        delay(80);
      }

      if(digitalRead(taster_minus) == LOW)
      {
        if(waageHight[index] > 80)
        waageHight[index] -= 5;
        delay(80);
      }

      if(digitalRead(taster_enter) == LOW)
      {
        delay(150);
        lcd.clear();
        phase = 1;
      }
    }

    else if(phase == 1)
    {
      lcd.setCursor(0,0);
      lcd.print("Waage:  ");
      lcd.print(nummer);
      lcd.print(" Max Last");

      lcd.setCursor(0,1);
      lcd.print("       ");
      lcd.print(gewichtOptionen[lastIndex[index]]);
      lcd.print(" kg   ");

      lcd.setCursor(0,2);

      if(lastIndex[index] > 0)
      {
        lcd.print("-");
        lcd.print(gewichtOptionen[lastIndex[index]-1]);
      }

      lcd.setCursor(16,2);

      if(lastIndex[index] < 5)
      {
        lcd.print("+");
        lcd.print(gewichtOptionen[lastIndex[index]+1]);
      }

      lcd.setCursor(3,3);
      lcd.print("Speichern: ENT");

      if(digitalRead(taster_plus) == LOW)
      {
        if(lastIndex[index] < 5)
          lastIndex[index]++;

        while(digitalRead(taster_plus)==LOW);
        delay(120);
      }

      if(digitalRead(taster_minus) == LOW)
      {
        if(lastIndex[index] > 0)
          lastIndex[index]--;

        while(digitalRead(taster_minus)==LOW);
        delay(120);
      }

      if(digitalRead(taster_enter) == LOW)
      {
        maxLast[index] = gewichtOptionen[lastIndex[index]];

        while(digitalRead(taster_enter)==LOW);
        delay(200);

        fertig = true;
      }
    }
  }
}

void waage_deaktivieren(int waagennummer)
{
  int index = waagennummer - 1;     // 0–11

  waageHight[index] = 0;
  maxLast[index] = 0;
}

void gewichteVerfahren_links(int waagenIndex)
{
  int aktuellesGewicht = 0;
  int maximalGewicht = maxLast[waagenIndex]; // Liest das Limit aus dem Array
  bool waageAktiv = true;                    // Steuert die Hauptschleife

  lcd.clear();
  
  // Die Schleife läuft, bis der Bediener JA zur nächsten Waage sagt
  while(waageAktiv == true)
  {
    lcd.setCursor(0,0);
    lcd.print("Waage: "); lcd.print(waagenIndex + 1);
    lcd.print(" Max:"); lcd.print(maximalGewicht); lcd.print("kg");

    lcd.setCursor(0,1);
    lcd.print("Ist: "); lcd.print(aktuellesGewicht); lcd.print("kg");

    lcd.setCursor(0,2);
    lcd.print("Waehlen: ");
    lcd.print(gewichtOptionen[gewichtOptionen_auswahl]);
    lcd.print("kg  ");

    lcd.setCursor(0,3);
    lcd.print("-    ENT    +    ^");

    //Ausgang aus der Funktion von Gewichten auflegen
    if(digitalRead(taster_zp) == LOW)
    {
      delay(150); // Entprellen für die Kasse
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Waage "); lcd.print(waagenIndex + 1); lcd.print(" fertig?");
      lcd.setCursor(0,2);
      lcd.print("+: Ja (Weiter)");
      lcd.setCursor(0,3);
      lcd.print("-: Nein (Bleiben)");

      bool entscheidungGetroffen = false;
      while(!entscheidungGetroffen)
      {
        // JA -> Wir wollen zur nächsten aktiven Waage
        if(digitalRead(taster_plus) == LOW)
        {
          // --Sicherheitsfahrt der Z-Achse ---
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Sicherheitsfahrt Z");
          lcd.setCursor(0,2);
          lcd.print("Bitte warten...");

          // Hier fährt die Z-Achse auf die sicheren 32000 Schritte nach oben.
                                        //z_verfahren_position(speedMotorZ, sicherheitshoehe); 
          // -----------------------------------------

          waageAktiv = false;        // Beendet die Schleife und verlässt die Funktion
          entscheidungGetroffen = true;
          delay(150);
        }
        
        // NEIN -> Wir wollen doch noch hier bleiben
        if(digitalRead(taster_minus) == LOW)
        {
          entscheidungGetroffen = true;
          lcd.clear(); 
          delay(150);
        }
      }
    }
  

    // Taster Plus (Auswahl der Gewichte)
    if(digitalRead(taster_plus) == LOW)
    {
      gewichtOptionen_auswahl++;
      if(gewichtOptionen_auswahl >= 6) gewichtOptionen_auswahl = 0;
      while(digitalRead(taster_plus)==LOW);
      delay(120);
    }

    // Taster Minus (Auswahl der Gewichtsklasse)
    if(digitalRead(taster_minus) == LOW)
    {
      gewichtOptionen_auswahl--;
      if(gewichtOptionen_auswahl < 0) gewichtOptionen_auswahl = 5;
      while(digitalRead(taster_minus)==LOW);
      delay(120);
    }

    // Taster Enter (Gewicht tatsächlich auflegen)
    if(digitalRead(taster_enter) == LOW)
    {
      while(digitalRead(taster_enter)==LOW);
      delay(120);

      int gewaehltesGewicht = gewichtOptionen[gewichtOptionen_auswahl];

      // PRÜFUNG: Maximalgewicht der aktuellen Waage beachten
      if((aktuellesGewicht + gewaehltesGewicht) <= maximalGewicht)
      {
        switch(gewichtOptionen_auswahl)
        {
          case 0: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_a);
          break;
          
          case 1: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_b); 
          break;

          case 2: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_c); 
          break;

          case 3: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_d); 
          break;

          case 4: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_e); 
          break;

          case 5: z_verfahren_position(speedGewichteAblegen, grenze_z-position_stufe_f); 
          break;
        }
        aktuellesGewicht += gewaehltesGewicht;
        
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Gewicht abgelegt!");
        delay(1000);
        lcd.clear();
      }
      else
      {
        // Fehler-Meldung bei Überladung
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ACHTUNG!");
        lcd.setCursor(0,1);
        lcd.print("Max. Last ueberschr!");
        delay(2000);
        lcd.clear();
      }
    }
  } // Ende der während waageAktiv-Schleife
}

void processLeftScaleRun()
{
      // -------------------- Waagen befahren--------------------
  
    if(konfiguration_waagen==true)
    {
      if(digitalRead(taster_minus)==HIGH)
      {
        lcd.setCursor(0,0);
        lcd.print("Wollen sie beginnen?");
        lcd.setCursor(0,2);
        lcd.print("+: Ja        Nein: -");
        if(digitalRead(taster_plus)==LOW)
        {
          lcd.clear();
          waagen_links_befahren=true;
        }
      }
      else
      {
        lcd.clear();
        konfiguration_waagen=false;
        auswahlWaagen();
      }
    }

    if(waagen_links_befahren==true)
    {
      lcd.setCursor(0,0);
      lcd.print("Fahre zur ersten");
      lcd.setCursor(0,1);
      lcd.print("aktivierten Waage");
      for(int i=0; i<=11; i++)
      {
        if(waageHight[i]!=0)
        {
          long xxx=0;
          switch (i)
          {
            case 0:
            x_verfahren_position(speedMotorX,position_waage_1);
            xxx = grenze_z - waageHight[i]-SCHRITT_Z*2;
            Serial.print("xxx: ");
            Serial.println(xxx);
            z_verfahren_position(speedMotorZ, (grenze_z - (waageHight[i]*SCHRITT_Z)-800));
            position_z_waage_1=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 1:
            x_verfahren_position(speedMotorX,position_waage_2);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_2=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 2:
            x_verfahren_position(speedMotorX,position_waage_3);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_3=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 3:
            x_verfahren_position(speedMotorX,position_waage_4);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_4=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 4:
            x_verfahren_position(speedMotorX,position_waage_5);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_5=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 5:
            x_verfahren_position(speedMotorX,position_waage_6);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_6=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 6:
            x_verfahren_position(speedMotorX,position_waage_7);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_7=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 7:
            x_verfahren_position(speedMotorX,position_waage_8);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_8=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 8:
            x_verfahren_position(speedMotorX,position_waage_9);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_9=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 9:
            x_verfahren_position(speedMotorX,position_waage_10);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_10=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 10:
            x_verfahren_position(speedMotorX,position_waage_11);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_11=ist_position_z;
            gewichteVerfahren_links(i);
            break;

            case 11:
            x_verfahren_position(speedMotorX,position_waage_12);
            z_verfahren_position(speedMotorZ,(grenze_z - waageHight[i]-SCHRITT_Z*2));
            position_z_waage_12=ist_position_z;
            gewichteVerfahren_links(i);
            break;
          }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Waage "); lcd.print(i+1); lcd.print(" fertig.");
        lcd.setCursor(0,1);
        lcd.print("Zur naechsten?");
        lcd.setCursor(0,3);
        lcd.print("+: Ja       -: Nein");
        bool eingabe_warte_schleife = true;
        while(eingabe_warte_schleife)
        {
          if(digitalRead(taster_plus) == LOW)
          {
            delay(200); // Entprellen
            eingabe_warte_schleife = false;     // Die for-Schleife läuft normal weiter zur nächsten Waage   
          }
          if(digitalRead(taster_minus) == LOW)
          {
            delay(200); // Entprellen
            eingabe_warte_schleife = false;
            waagen_links_befahren = false; // Bricht den Gesamtvorgang ab
            manuellerModus = true;         // Geht zurück in den manuellen Modus
            break;                         // Bricht die while-Schleife ab
          }
        }
            // Wenn der Nutzer "Nein" gedrückt hat, auch die for-Schleife abbrechen
          if(waagen_links_befahren == false) 
          {
            break; 
          }
        }       // Ende der if(waageHight[i]!=0) Bedingung
      }         // Ende der for(int i=0; i<=11; i++) Schleife

      // Wenn er alle Waagen befahren hat (Schleife ist komplett durch):
      if(waagen_links_befahren == true) 
      {
        waagen_links_befahren = false;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Alle Waagen beendet");
        delay(2000);
        manuellerModus = true;
      }
    }
}
