#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "Dauerbelastung.h"

void x_verfahren(int richtung, int zeit_x)
{
    zaehler_rampe = 0.0;
    digitalWrite(en_x, LOW);            //enable an
    delay(100);

    if(richtung == plus)
    {
      digitalWrite(dir_x, LOW);
        while(((digitalRead(taster_xp) == LOW && digitalRead(endschalter_xp) == LOW) && manuellerModus== true) || ((referenzfahrt_x == true) && digitalRead(endschalter_xn) == HIGH) || endschaltertest_verfahren==true)
        {
            digitalWrite(pul_x, HIGH);
            int delaywert = zeit_x - zaehler_rampe;
            if(delaywert < 0) delaywert = 0;
            delayMicroseconds(delaywert);
            digitalWrite(pul_x, LOW);
            delayMicroseconds(delaywert);

            ist_position_x++;
            if(zaehler_rampe <= grenze) zaehler_rampe += 0.5;
        }
    }
    else if(richtung == minus)
    {
      digitalWrite(dir_x, HIGH);
      delayMicroseconds(50);   
        while ((digitalRead(taster_xn) == LOW && digitalRead(endschalter_xn) == LOW && manuellerModus== true) || (referenzfahrt_x == true && digitalRead(endschalter_xn) == LOW) || endschaltertest_verfahren==true)
        {
            digitalWrite(pul_x, HIGH);
            int delaywert = zeit_x - zaehler_rampe;
            if(delaywert < 0) delaywert = 0;
            delayMicroseconds(delaywert);
            digitalWrite(pul_x, LOW);
            delayMicroseconds(delaywert);
            ist_position_x--;
            if(zaehler_rampe <= grenze) zaehler_rampe += 0.5;
        }
    }
}

void z_verfahren(int richtung, int zeit_z)
{
    zaehler_rampe = 0.0;
    digitalWrite(en_z, LOW);            //enable an
    delay(100);
    digitalWrite(motorbremse, HIGH);    //Motorbremse deaktivieren
    delay(200);

    if(richtung == plus)
    {
        digitalWrite(dir_z, LOW);
        while((digitalRead(taster_zp) == LOW && digitalRead(endschalter_zp) == LOW && manuellerModus== true) ||
          ((referenzfahrt_z == true) && digitalRead(endschalter_zp) == LOW) || endschaltertest_verfahren==true)
        {
            digitalWrite(pul_z, HIGH);
            int delaywert = zeit_z - zaehler_rampe;
            if(delaywert < 0) delaywert = 0;
            delayMicroseconds(delaywert);
            digitalWrite(pul_z, LOW);
            delayMicroseconds(delaywert);

            ist_position_z--;
            if(zaehler_rampe <= grenze) zaehler_rampe += 0.5;
        }
    }
    else if(richtung == minus)
    {
        digitalWrite(dir_z, HIGH);

        while((digitalRead(taster_zn) == LOW && digitalRead(endschalter_zn) == LOW && manuellerModus== true) ||
          ((referenzfahrt_z == true) && digitalRead(endschalter_zp) == HIGH) || endschaltertest_verfahren==true)
        {
            digitalWrite(pul_z, HIGH);
            int delaywert = zeit_z - zaehler_rampe;
            if(delaywert < 0) delaywert = 0;
            delayMicroseconds(delaywert);
            digitalWrite(pul_z, LOW);
            delayMicroseconds(delaywert);

            ist_position_z++;
            if(zaehler_rampe <= grenze) zaehler_rampe += 0.5;
        }
    }
    zStart=millis();        //Zeit nach der letzten Bewegung der Z-Achse Merken
}

void verfahren_koordinaten_nach_wunsch_x()
{
  lcd.clear();
  while(true)
  {
    lcd.setCursor(0,0);
    lcd.print("X Achse soll    ");
    lcd.setCursor(0,1);
    lcd.print(millimeter_x);
    lcd.print(" mm fahren");
    lcd.setCursor(0,3);
    lcd.print("-    ENT     +");

    if (digitalRead(taster_plus) == LOW)
    {
      delay(250);
      millimeter_x += 50;
    }

    if (digitalRead(taster_minus) == LOW)
    {
      delay(250);
      millimeter_x -= 50;
    }

    if (digitalRead(taster_enter) == LOW)
    {
      delay(250);
      long schritte = millimeter_x * SCHRITT_X;
      x_verfahren_position(speedMotorX, schritte);
      break;   // Menü verlassen
    }
  }
}

void x_verfahren_position(int zeit_x, long soll_position_x) 
{ 
  zaehler_rampe = 0.0; 
  digitalWrite(en_x, LOW);               //enable an 
  delay(100);
  if(soll_position_x>ist_position_x)    //Wenn man in plus Richtung fahren muss 
  { 
    digitalWrite(dir_x, LOW);           //richtung plus

    while((ist_position_x < soll_position_x) && digitalRead(endschalter_xp)==LOW) 
    { 
      digitalWrite(pul_x, HIGH); 
      delayMicroseconds(speedMotorX-zaehler_rampe); 
      digitalWrite(pul_x, LOW); 
      delayMicroseconds(speedMotorX-zaehler_rampe);

      ist_position_x++; 

      if ((zaehler_rampe <= grenze) && ((soll_position_x - ist_position_x) >= 500)) 
        { 
          zaehler_rampe = zaehler_rampe + 0.5;      //schneller werden ab 
        } 
      if ((soll_position_x - ist_position_x) < 500) 
        { 
          zaehler_rampe = zaehler_rampe - 0.5;      //Wieder abbremsen
        } 
    } 
  } 
  else if (ist_position_x > soll_position_x) 
  {
    zaehler_rampe = 0.0; 
    digitalWrite(dir_x, HIGH);

    while((ist_position_x > soll_position_x) && digitalRead(endschalter_xn)==LOW) 
    {
      digitalWrite(pul_x, HIGH); 
      delayMicroseconds(speedMotorX - zaehler_rampe);
      digitalWrite(pul_x, LOW); 
      delayMicroseconds(speedMotorX - zaehler_rampe);
      ist_position_x--; 
      if ((zaehler_rampe <= grenze) && ((ist_position_x - soll_position_x) >= 500)) 
      {
        zaehler_rampe = zaehler_rampe + 0.5; 
      } 
      if ((ist_position_x - soll_position_x) < 500) 
      { 
        zaehler_rampe = zaehler_rampe - 0.5; 
      } 
    } 
  }
  digitalWrite(en_x, HIGH);
}

void verfahren_koordinaten_nach_wunsch_z()
{
  while(true)
  {
    lcd.setCursor(0,0);
    lcd.print("Z Achse soll    ");
    lcd.setCursor(0,1);
    lcd.print(millimeter_z);
    lcd.print(" mm     ");

    if (digitalRead(taster_plus) == LOW)
    {
      delay(250);
      millimeter_z += 10;
    }

    if (digitalRead(taster_minus) == LOW)
    {
      delay(250);
      millimeter_z -= 10;
    }

    if (digitalRead(taster_enter) == LOW)
    {
      delay(250);
      long schritte = millimeter_z * 160;
      z_verfahren_position(speedMotorZ, schritte);
      break;   // Menü verlassen
    }
  }
}

void z_verfahren_position(int zeit_z, long soll_position_z) 
{ 
  zaehler_rampe = 0.0; 
  digitalWrite(en_z, LOW);
  delay(100); 
  digitalWrite(motorbremse, HIGH);
  delay(200); 

  if(soll_position_z > ist_position_z)          //Plus, nach unten
  { 
    digitalWrite(dir_z, HIGH);
    while((soll_position_z > ist_position_z) && digitalRead(endschalter_zn)==LOW)       //Fahre solange die ist_position den wert von soll_position erreicht
    { 
      digitalWrite(pul_z, HIGH); 
      delayMicroseconds(zeit_z-zaehler_rampe); 
      digitalWrite(pul_z, LOW); 
      delayMicroseconds(zeit_z-zaehler_rampe);

      ist_position_z++; 

      if ((zaehler_rampe <= grenze) && ((soll_position_z - ist_position_z) >= 500))
        zaehler_rampe += 0.5;

      if ((soll_position_z - ist_position_z) < 500)
        zaehler_rampe -= 0.5;
    } 
  } 
  else if (soll_position_z < ist_position_z) 
  {
    zaehler_rampe = 0.0; 
    digitalWrite(dir_z, LOW);

    while ((soll_position_z<ist_position_z) && digitalRead(endschalter_zn)==LOW)
    {
      digitalWrite(pul_z, HIGH); 
      delayMicroseconds(zeit_z - zaehler_rampe);
      digitalWrite(pul_z, LOW); 
      delayMicroseconds(zeit_z - zaehler_rampe);

      ist_position_z--; 

      if ((zaehler_rampe <= grenze) && ((ist_position_z - soll_position_z) >= 500))
        zaehler_rampe += 0.5;

      if ((ist_position_z - soll_position_z) < 500)
        zaehler_rampe -= 0.5;
    } 
  }
  zStart=millis();
}

void nullpunkt_z()
{
  lcd.setCursor(15, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("Nullpunkt z gesetzt!");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
  Serial.print("Z Achse wurde auf Wert: ");
  Serial.print(ist_position_z);
  Serial.print("gesetzt");
  delay(5000);
  ist_position_z = ist_nullpunkt_z;
  
  manuellerModus=false;
  lcd.clear();
  nullpunkt_gesetzt=true;
  gewichteVerfahren_dauerbelastung();
}

void processZBrake()
{
    if (zStart > 0)                         // Z-Achse bewegt
    {
      if ((millis()- zStart)> zBremsWait)   // Z-Achse  seit zBremsWait(ms) nicht bewegt
      {
        digitalWrite(motorbremse, LOW);     // Motorbremse aktivieren
        delay(200);
        digitalWrite(en_z, HIGH);         //Motortreiber Stromlos machen

        zStart = 0;
      }
    }
}
