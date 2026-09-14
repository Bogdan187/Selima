#include <Arduino.h>
#include "Config.h"
#include "Globals.h"
#include "MotorControl.h"
#include "SerialInterface.h"
#include "ReferenceRun.h"

void serialEvent3() 
{
 // Serial.println("und ich warte auf ein Zeichen");
  while (Serial3.available())
  {
    // get the new byte:
    char inChar = (char)Serial3.read();
    if (inChar == '\n')
    {
      // LF nach CR ignorieren.
      continue;
    }
    // Eingaben begrenzen, damit ein fehlerhafter Sender den Speicher nicht
    // unkontrolliert mit einem Arduino-String fuellt.
    if (inputString.length() < 32)
    {
      inputString += inChar;
    }
    else
    {
      inputString = "";
      stringComplete = false;
      continue;
    }
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') 
    {
      stringComplete = true;
    }
  }
}

bool serialCommandHasRequiredLength(char command)
{
  switch (command)              //Wie die Länge des eingegebens Wortes Minimal sein darf
  {
    case 'r': return inputString.length() >= 4;
    case 'x': return inputString.length() >= 5;
    case 'n': return inputString.length() >= 3;
    case 'a': return inputString.length() >= 4;
    case 'd': return inputString.length() >= 5;
    case 'o': return inputString.length() >= 4;
    case 'z': return inputString.length() >= 3;
    case 'v': return inputString.length() >= 4;
    case 's': return inputString.length() >= 4;
    default: return false;
  }
}

void processSerialCommands()
{
    // print the string when a CR arrives:
    if (stringComplete) 
    {

      Serial.println(inputString);
      if (inputString.length() > 0 && serialCommandHasRequiredLength(inputString[0]) &&
          (inputString[0] == 'r' || inputString[0] == 'x' || inputString[0] == 'z'
          ||inputString[0] == 'o' ||inputString[0] == 'n' || inputString[0]== 'a' || inputString[0] == 'd' || inputString[0] == 'v' || inputString[0] == 's'))
      {
        serielle_schnittstelle=true;
        switch (inputString[0])
        {           
          case 'r': // Referenzfahrt
          {
            if (inputString[1]== '0' && inputString[2]== '0' && inputString[3]== '0' )
            {
              x_referenzfahrt_skip=false;
              referenzfahrt_done=false;
              referenzfahrt = true;
              referenzfahrt_anzeige_0 = true;
              delay(1000);
              taster_enter_simuliert=true;
              inputString = "";
              stringComplete = false;
            }
            else
            {
              inputString = "";
              Serial3.print("R-\r");
              stringComplete = false;
            }
          }
          break;

          case 'x': // X-Achse xxxx = Position in mm
          {
            Wegx =  ((inputString[1]-'0') * 1000) +
                    ((inputString[2]-'0') * 100) +
                    ((inputString[3]-'0') * 10) +
                    ((inputString[4]-'0'));

            NeuePosx = Wegx * SCHRITT_X;


            // Grundbedingungen für eine X-Bewegung prüfen
            if ((digitalRead(endschalter_xp) == LOW) &&
                (digitalRead(endschalter_xn) == LOW) &&
                (NeuePosx >= 0) &&
                (referenzfahrt_done == true))
            {
              // Z-Referenzfahrt bearbeiten
              referenzfahrt_z_vor_x();


              /* X darf ERST fahren, wenn die komplette
              Z-Referenzfahrt abgeschlossen ist*/

              if (z_phase_referenzfahrt == 2)
              {
                x_verfahren_position(speedMotorX, NeuePosx);

                // Für den nächsten X-Befehl zurücksetzen
                z_phase_referenzfahrt = 0;

                inputString = "";
                Serial3.print("X+\r");
                stringComplete = false;
              }

              /* WICHTIG:
              Wenn z_phase_referenzfahrt noch 0 oder 1 ist,
              wird inputString NICHT gelöscht.
              Dadurch wird der X-Befehl im nächsten loop()
              erneut verarbeitet.*/
            }
            else
            {
              // Fehler -> auch Z-Referenzstatus zurücksetzen
              z_phase_referenzfahrt = 0;
              referenzfahrt_z = false;

              inputString = "";
              Serial3.print("X-\r");
              stringComplete = false;
            }
          }
            break;

          case 'n':
          {
            AnzSch =  ((inputString[1]-'0')*10) +
                      ((inputString[2]-'0')*1);
            if(AnzSch != 0)
            {
              inputString = "";
              Serial3.print("N+\r");
              stringComplete = false;
            }
            else
            {
              inputString = "";
              Serial3.print("N-\r");
              stringComplete = false;
            }
          }
          break;

          case 'a':
          {
            long gap_scheiben_eingabe =   ((inputString[1]-'0')*1000) +       //Format xx,xx mm
                                          ((inputString[2]-'0')*100) +
                                          ((inputString[3]-'0')*10)+
                                            ((inputString[4]-'0')*1);

            long gap_scheiben_eingabe_mm= gap_scheiben_eingabe / 100.0;  // Umwandlung in z.zzz Format

            if(gap_scheiben_eingabe >= 5)
            {
              gap_scheiben_schritte= (gap_scheiben_eingabe_mm* SCHRITT_Z);
              inputString = "";
              Serial3.print("A+\r");
              stringComplete = false;
            }
            else
            {
              inputString = "";
              Serial3.print("A-\r");
              stringComplete = false;
            }
          }
          break;

          case 'd':
          {
                 abstand_boden_gewichtskette =   ((inputString[1]-'0')*1000) +      //Abstand zwischen den niedrigsten Kettenglied
                                                 ((inputString[2]-'0')*100) +       //und den Boden der Maschine
                                                 ((inputString[3]-'0')*10)+
                                                 ((inputString[4]-'0')*1);


            if(abstand_boden_gewichtskette>abstand_boden_gewichtskette_max)       //> als 200 mm.
            {
              abstand_boden_gewichtskette_schritte=(abstand_boden_gewichtskette*SCHRITT_Z);
              inputString = "";
              Serial3.print("D+\r");
              stringComplete = false;
            }
            else
            {
              inputString = "";
              Serial3.print("D-\r");
              stringComplete = false;
            }

          }
          break;

          case 'o':
          {
            position_offset =   ((inputString[1]-'0')*100) +        //Höhe Wage in xxx millimeter
                                ((inputString[2]-'0')*10)+
                                ((inputString[3]-'0')*1);

            if(position_offset< abstand_boden_gewichtskette && position_offset!=0)                  //Prüfen ob die Waage nicht höher ist als der
            {                                                                                       //Abstand von Gewichtskette zum Boden
              position_offset_schritte= (position_offset*SCHRITT_Z);                                //umrechnen von Offset in Schritte
              inputString = "";
              Serial3.print("O+\r");
              stringComplete = false;
            }
            else
            {
              inputString = "";
              Serial3.print("O-\r");
              stringComplete = false;
            }
          }
          break;

          case 'z':                                     //Gewichte ablegen
          {
            auswahl_gewicht = ((inputString[1]-'0')*10) +
                              ((inputString[2]-'0')*1);


            if(auswahl_gewicht<=AnzSch)         //Man kann nicht mehr Scheiben auflegen als davor angegeben
            {
                  long strecke=(((auswahl_gewicht-0.5)*gap_scheiben_schritte)+(abstand_boden_gewichtskette_schritte-position_offset_schritte));

                  z_verfahren_position(speedGewichteAblegen, strecke);

                  inputString = "";
                  Serial3.print("Z+\r");
                  stringComplete = false;
            }

            else
            {
              inputString = "";
              Serial3.print("Z-\r");
              stringComplete = false;
            }
          }
          break;

          case 'v':
          {
            spannung =  ((inputString[1]-'0')*100) +
                          ((inputString[2]-'0')*10)+
                          ((inputString[3]-'0')*1);
            if (spannung == 1 || spannung==0)
            {
              if (spannung==1)
              {
              digitalWrite(spannungsversorgung24, HIGH);           //Anschalten der Spannungen an den Wägezellen
              digitalWrite(spannungsversorgung12, HIGH);
              inputString = "";
              Serial3.print("V+\r");
              stringComplete = false;
              }
              if(spannung == 0)
              {
              digitalWrite(spannungsversorgung24, LOW);            //Ausschalten der Spannungen an den Wägezellen
              digitalWrite(spannungsversorgung12, LOW);
              inputString = "";
              Serial3.print("V+\r");
              stringComplete = false;
              }
            }
            else
            {
              inputString = "";
              Serial3.print("V-\r");
              stringComplete = false;
            }
          }
          break;

          case 's':
          {
             zustandRollo =   ((inputString[1]-'0')*100) +
                              ((inputString[2]-'0')*10)+
                              ((inputString[3]-'0')*1);
            if (zustandRollo == 1 || zustandRollo==0)
            {
              if (zustandRollo==1)
              {
              digitalWrite(rollo_up, HIGH);           //Anschalten der Spannungen an den Wägezellen
              delay(1000);
              digitalWrite(rollo_up, LOW);
              inputString = "";
              Serial3.print("S+\r");
              stringComplete = false;
              }
              if(zustandRollo == 0)
              {
              digitalWrite(rollo_down, HIGH);
              delay(1000);                          //Ausschalten der Spannungen an den Wägezellen
              digitalWrite(rollo_down, LOW);
              inputString = "";
              Serial3.print("S+\r");
              stringComplete = false;
              }
            }
            else
            {
              inputString = "";
              Serial3.print("S-\r");
              stringComplete = false;
            }
          }
          break;

        }
      }
      else
      {
        inputString = "";
        Serial3.print("#9\r");
        if (bMitAnz)
        {
          lcd.setCursor(0,3);
          lcd.print("Falscher Bef. ");
        }
      }
    }
}