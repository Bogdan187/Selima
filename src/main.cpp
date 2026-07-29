// -------------------- Initialisierung --------------------
//  20.11.23  H.Witte Serielle Schnittstelle hinzu, Schritte angepasst
//
//  18.02.26 B.Saeisih Umschreibung des Codes für die 2 Achs Celima Maschine
//
//

/*================================================= Stand der Entwicklung:  Serielle Befehle sind fertig und funktionieren wie erwartet.
                                                    Die Bedienung über den Steuerpult ist auch fertig bis auf:      1. Manueller Modus funktionert einwandfrei                                                                      
                                                                                                                    2. Der Custom Modus (davor Bereich 2) ist gedacht zu befahren einer Waage und mit später noch einer Funktion von dauerbelastungstest  
                                                                                                                    3. Auswahl Waagen ist eine Idee gewesen die sich nicht umgesetzt hat und deswegen wird da noch weiter geschraubt. Der jetzige Stand ist noch sehr "Hard-codiert".

*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// -------------------- Funktionsdeklarationen für C++/PlatformIO --------------------
void x_verfahren(int richtung, int zeit_x);
void z_verfahren(int richtung, int zeit_z);
void verfahren_koordinaten_nach_wunsch_x();
void x_verfahren_position(int zeit_x, long soll_position_x);
void verfahren_koordinaten_nach_wunsch_z();
void z_verfahren_position(int zeit_z, long soll_position_z);
void nullpunkt_z();
void gewichteVerfahren_dauerbelastung();
void auswahlWaagen();
void eigenschaftenWaage(int nummer);
void waage_deaktivieren(int waagennummer);
void gewichteVerfahren_links(int waagenIndex);
void serialEvent3();
void konfiguration_waage_rechts();
void gewichte_auflegen_rechts();
void gewichte_auflegen_rechts_exit();
// -------------------------------------------------------------------------------


LiquidCrystal_I2C lcd(0x27, 20, 4);

#define plus 0
#define minus 1

#define SCHRITT_X   160
#define SCHRITT_Z   160


#define maxScheiben 6


// -------------------- Tastenfeld --------------------
const int taster_zp = 46;                                   //  ↑
const int taster_zn = 47;                                   //  ↓
const int taster_xp = 48;                                   //  ←              
const int taster_xn = 49;                                   //  →
const int taster_plus = 42;                                 //  +
const int taster_minus =43;                                 //  - 
const int taster_0 =44;                                     //  0
const int taster_enter = 45;                                //  ENT

bool taster_zp_simuliert = false;                           //  ↑
bool taster_zn_simuliert = false;                                   //  ↓
bool taster_xp_simuliert = false;                                   //  ←              
bool taster_xn_simuliert = false;                                   //  →
bool taster_plus_simuliert = false;                                 //  +
bool taster_minus_simuliert = false;                                 //  - 
bool taster_0_simuliert = false;                                     //  0
bool taster_enter_simuliert = false;  

// -------------------- Endschalter --------------------
const int endschalter_xp = 40;                              // Rechts Oben
const int endschalter_xn = 39;                              // Links Oben
const int endschalter_zp = 52;                              // Oben
const int endschalter_zn = 53;                              // Unten
// ----------------------------------------

// -------------------- Motorsteuerkarte x-Achse --------------------
const int dir_x = 9;                          //Richtung
const int pul_x = 10; 
const int en_x = 11;
// ----------------------------------------

// -------------------- Motorsteuerkarte z-Achse --------------------
const int dir_z = 6;
const int pul_z = 7; 
const int en_z = 8;
// ----------------------------------------

// -------------------- Motorbremse --------------------
const int motorbremse = 22;
// ----------------------------------------

//------------Spannungsversorgung von Wägezellen-----------------
const int spannungsversorgung24 = 24;
const int spannungsversorgung12 = 25;
int spannung=0;
//---------------------------------------------------------------

//------------------Steuerrung von Sichtschutz/Rollos------------
const int rollo_up = 26;
const int rollo_down = 27;
int zustandRollo=0;
//-----------------------------------------------------------------

// -------------------- Positionen für Gewichte --------------------
const int position_stufe_a = 1140;    // 2KG
const int position_stufe_b = 2480;    // 2+2 KG = 4
const int position_stufe_c = 4180;    // 2+2+1 KG = 5
const int position_stufe_d = 5380;    // 2+2+1+3 KG = 8
const int position_stufe_e = 6580;    // 2+2+1+3+5 KG = 13
const int position_stufe_f = 8580;    // 2+2+1+3+5+5 KG = 18
// --------------------------------------------------

//-----------------------Positionen der Waagen ----------------------
const long grenze_z_kabel= 88000;
const long grenze_z= 98000;
const long grenze_x_tischanfang= 275000;
const long grenze_x_tischende= 291000;


long hoehe_waage_rechts=100;
 long x_koordinate_waage_rechts= 1000;

long abstand_kette_boden= 500;     //Standart Wert ist 50 cm (500mm * SCHRITTE_Z)

static int anzahl_scheiben_rechts=4;

float gap_scheiben_rechts=8.0;
const long position_waage_1= 275000;//0;        //  kann noch nicht befahren werden
long position_z_waage_1=0;
long position_z_waage_2=0;
long position_z_waage_3=0;
long position_z_waage_4=0;
long position_z_waage_5=0;
long position_z_waage_6=0;
long position_z_waage_7=0;
long position_z_waage_8=0;
long position_z_waage_9=0;
long position_z_waage_10=0;
long position_z_waage_11=0;
long position_z_waage_12=0;
const long position_waage_2= 291000;//16000;        //Für Testing da keine einzige Waage noch drin ist in der Maschine.
const long position_waage_3= 286000;//34000;
const long position_waage_4= 286000;//52000;
const long position_waage_5= 286000;//70000;
const long position_waage_6= 286000;//86000;
const long position_waage_7= 286000;//104000;
const long position_waage_8= 286000;//122000;
const long position_waage_9= 286000;//140000;
const long position_waage_10= 286000;//158000;
const long position_waage_11= 286000;//176000;
const long position_waage_12= 286000;//194000;

long position_waage_rechts=100;
long position_z_waage_rechts_koordinate=0;
long position_x_waage_rechts_koordinate=0;

//----------------Zeit für Motor---------------

static int speedMotorX= 700;                  //Für Manuellen Betrieb
static int speedMotorReferenzfahrt_X= 1200;
static int speedMotorReferenzfahrtNegativ_X= 700;    

static int speedMotorZ= 700;                  //Für Manuellen Betrieb
static int speedMotorReferenzfahrt_Z= 700; 
static int speedMotorReferenzfahrtNegativ_Z= 1200;

static int speedGewichteAblegen= 1500;

// -------------------- Status ----------------------
int z_phase=0;
int x_phase=0;
bool referenzfahrt_anzeige_0 = true;
bool referenzfahrt_anzeige_1 = false;
bool referenzfahrt_anzeige_fertig = false;
bool referenzfahrt = true;
bool referenzfahrt_x = false;
bool referenzfahrt_z = false;
bool referenzpunkt_x = false;
bool referenzpunkt_z = false;
bool referenzfahrt_z_done=false;
bool manuellerModus = false;
bool referenzfahrt_done = false;
bool funktion_auswahlWaagen= false;
bool auswahlWaagen_status=false;
bool konfiguration_waagen = false;
bool position_verfahren_modus=false;
bool position_erreicht=false;
bool nullpunkt_gesetzt=false;
bool dauerbelastungstest=false;
bool waagen_links_befahren=false;
bool x_referenzfahrt_skip=false;
bool referenzfahrt_schnittstelle= false;
bool konfiguration_waage_rechts_fertig= false;
bool serielle_schnittstelle=false;
// ----------------------------------------------------

//----------------------Auswahl Waagen-----------------

const int eins=1;

int wahl_gewichtskette=0;

int gewichte_drauf_rechts=1;

int auswahl_gewicht=0;

int anzahl_gewichtsscheiben=6;

int waagennummer= 0;

int gewichtskette[] = {2, 2, 1, 3, 5, 5};                                // Gewichtskette: Einzelgewichte, die aufsummiert werden

long z_achse_gesammt= 144000;

long abstand_boden_gewichtskette_schritte=0;



int arrayGroesse = sizeof(gewichtskette) / sizeof(gewichtskette[0]);  


int waageHight[12] = {100,100,100,100,100,100,100,100,100,100,100,100};    // Höhe der Waagen 


int index_waage[12] = {0,0,0,0,0,0,0,0,0,0,0,0};                          // Index für die Gewichtskette


int maxLast[12] = {0,0,0,0,0,0,0,0,0,0,0,0};                              // Benutzer gespeicherte Maximallast pro Waage


int gewichtOptionen[] = {2, 4, 5, 8, 13, 18, 0};
int anzahlOptionen = 6;

int lastIndex[12] = {0};                                                // merkt sich Auswahl je Waage

static int gewichtOptionen_auswahl=0;

// -------------------- Positionen --------------------

long ist_position_x = 0.0;
long ist_position_z = 0.0;
long ist_nullpunkt_z= 0.0;

long position_gewicht = 0;


long millimeter_x = 0;
long millimeter_z = 0;

long Wegx = 0;
long NeuePosx = 0;
long Posz = 0;
long NeuePosz = 0;
long Nullz = 0;
bool Offsetz = false;

long AnzSch = 0;
long AnzSchS = 0;

long gap_scheiben_eingabe = 0;                                                      //Abstand Scheiben in 1/10. mm
long gap_scheiben_schritte = 0;

long DiffZ0 = 330;                                                                  // Abstand Boden zu Unterkante Gewichtsstapel im Nullpunkt
long DiffZ0S = 0;

long hoehe_waage=0;
long hoehe_waage_schritte=0;
long abstand_boden_gewichtskette=0;
long position_offset=0;

// ----------------------------------------

// -------------------- Rampe --------------------

float zaehler_rampe = 0.0;

const int grenze= 550;
// ----------------------------------------

//------------------------ Serielle Schnittstelle --------------
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool bMitAnz = false;
bool ReferenzSchnittstelle = false;
//---------------------------------------------------------------

// --------------------- Automatisches Bremsen Z-Achse -----------------------
unsigned long zStart = 0;
#define zBremsWait 10000                  //Die Zeit zum ausführen von Automatischer Motorbremse




//================================= Set up  ==========================================

void setup()
{
  // -------------------- Tastenfeld --------------------
  pinMode(taster_xn, INPUT_PULLUP);
  pinMode(taster_xp, INPUT_PULLUP);
  pinMode(taster_enter, INPUT_PULLUP);
  pinMode(taster_0, INPUT_PULLUP);
  pinMode(taster_zn, INPUT_PULLUP);
  pinMode(taster_zp, INPUT_PULLUP);
  pinMode(taster_plus, INPUT_PULLUP);
  pinMode(taster_minus, INPUT_PULLUP);
  // ----------------------------------------

  // -------------------- Endschalter --------------------
  pinMode(endschalter_xp, INPUT);                       //High wenn betätigt wird
  pinMode(endschalter_xn, INPUT);
  pinMode(endschalter_zp, INPUT_PULLUP);                //PULLUP damit auch High wenn betätigt wird.
  pinMode(endschalter_zn, INPUT_PULLUP);
  // ----------------------------------------

  // -------------------- Motorsteuerkarte x-Achse --------------------
  pinMode(dir_x, OUTPUT); 
  pinMode(pul_x, OUTPUT);
  pinMode(en_x, OUTPUT);
  // ----------------------------------------

  // -------------------- Motorsteuerkarte z-Achse --------------------
  pinMode(dir_z, OUTPUT); 
  pinMode(pul_z, OUTPUT);
  pinMode(en_z, OUTPUT);
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

  Serial3.begin(9600, SERIAL_8N1);
    // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  // -------------------- LCD-Display (20x4) --------------------
  Serial.begin(9600);
  // ----------------------------------------
}

// ==================== Aus Selima_BS_functions.ino ====================

// --------------------------    X-Achse Verfahren      ----------------------------

void x_verfahren(int richtung, int zeit_x)
{
    zaehler_rampe = 0.0;
    digitalWrite(en_x, LOW);            //enable an
    delay(100);
    digitalWrite(motorbremse, HIGH);    //Motorbremse deaktivieren
    delay(200);

    if(richtung == plus)
    {
      digitalWrite(dir_x, LOW);
        while(((digitalRead(taster_xp) == LOW && digitalRead(endschalter_xp) == LOW) && manuellerModus== true) || ((referenzfahrt_x == true) && digitalRead(endschalter_xn) == HIGH))
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
        while ((digitalRead(taster_xn) == LOW && digitalRead(endschalter_xn) == LOW && manuellerModus== true) || (referenzfahrt_x == true && digitalRead(endschalter_xn) == LOW))
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
    digitalWrite(motorbremse, LOW);
}

//---------------------------------------------------------------------------------

// -----------------------------   Z-Achse Verfahren  ------------------------------
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
        while((digitalRead(taster_zp) == LOW && digitalRead(endschalter_zp) == LOW && manuellerModus== true) || ((referenzfahrt_z == true) && digitalRead(endschalter_zp) == LOW))
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

        while((digitalRead(taster_zn) == LOW && digitalRead(endschalter_zn) == LOW && manuellerModus== true) || ((referenzfahrt_z == true) && digitalRead(endschalter_zp) == HIGH))
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
    digitalWrite(motorbremse, LOW);
}

//---------------------------------------------------------------------------------------


//-----------------------------   Eingang zur Verfahren Positionen X  -----------------

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


//---------------------------   Positionen Verfahren X    --------------------------------

void x_verfahren_position(int zeit_x, long soll_position_x) 
{ 
  zaehler_rampe = 0.0; 
  digitalWrite(en_x, LOW);               //enable an 
  delay(100); 
  digitalWrite(motorbremse, HIGH);      //Motorbremse deaktivieren 
  delay(200); 
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
//----------------------------------------------------------------------------------------------------#

//------------------------------    Eingang Verfahren Positionen Z  ----------------------------------

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
//-----------------------------------------------------------------------------------------------------


//------------------------------    Positionen Verfahren Z    ----------------------------------------

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
    while((soll_position_z > ist_position_z) && digitalRead(endschalter_zp)==LOW)       //Fahre solange die ist_position den wert von soll_position erreicht
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

    while(soll_position_z<ist_position_z)
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

  digitalWrite(en_z, HIGH);
}
//------------------------------------------------------------------------------------------------------------------

//----------------------------------------------  Nullpunkt Setzen  ------------------------------------------------

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
//------------------------------------------------------------------------------------------------------------------

//------------------------------------    Gewichtsscheiben Auswahl      ---------------------------------------
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
//---------------------------------------------------------------------------------------------------


//----------------------------------      Auswahl Waagen      ---------------------------------------

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
//-------------------------------------------------------------------------------------------------------

//-----------------------------    Eingenschaften Waage     ---------------------------------------------

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
//----------------------------------------------------------------------------------

//-------------------------   Waagen Deaktivieren falls nicht in benutzung      -----------

void waage_deaktivieren(int waagennummer)
{
  int index = waagennummer - 1;     // 0–11

  waageHight[index] = 0;
  maxLast[index] = 0;
}

//---------------------------------------------------------------------------------

//------------------------------  Gewichte auflegen links --------------------------
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

//-----------------------------   Serialle Schnittstelle    -----------------------------------

void serialEvent3() 
{
 // Serial.println("und ich warte auf ein Zeichen");
  while (Serial3.available())
  {
    // get the new byte:
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString += inChar;
 //   Serial.println(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') 
    {
      stringComplete = true;
    }
  }
}


//------------------------------- Der Rechte Bereich konfig / benutzung / Custom ------------------------------
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
      lcd.print("-: -2          +: +2");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT     ");
      if (digitalRead(taster_plus) == LOW) 
      { 
       hoehe_waage_rechts+= 2; 
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW) 
      { 
        if (hoehe_waage_rechts> 50)                                 // Mindesthöhe
        {
          hoehe_waage_rechts-=2;
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
      lcd.print("-: -5          +: +5");
      lcd.setCursor(0, 3);
      lcd.print("     Enter: ENT     ");
      if (digitalRead(taster_plus) == LOW) 
      { 
       abstand_kette_boden+= 5; 
        delay(20); 
      }
      if (digitalRead(taster_minus) == LOW) 
      { 
        abstand_kette_boden-=5;
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
      lcd.print("Breite:  "); lcd.print(gap_scheiben_rechts);
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
  while(digitalRead(taster_zp)==HIGH)
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
      lcd.print("     Enter: ENT     ");
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

// ======================= Aus Selima_BS_loop.ino ======================

void loop() 
{
  // -------------------- Referenzfahrt --------------------
  if ((referenzfahrt_anzeige_0) == true && (referenzfahrt) == true)                 //Standart am Anfang
  { lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Referenzfahrt");
    lcd.setCursor(0,1);
    lcd.print("bereit!");
    lcd.setCursor(0,2);
    lcd.print("Bitte ENT");
    lcd.setCursor(0,3);
    lcd.print("Taste halten!"); 
    //Serial.println("Willkommen. Warten auf Eingabe. Für Referenzfahrt = ENT");                                                                 //Output
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
        Serial.println("Referenzfahrt abgeschlossen.");
        taster_enter_simuliert=false;
        digitalWrite(motorbremse, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Referenzfahrt");
        lcd.setCursor(0,1);
        lcd.print("abgeschlossen");
        Serial3.print("R+\r");
        ist_position_x=0;
        referenzfahrt_x = false;
        x_phase = 0;
        referenzfahrt_anzeige_fertig = true;
        referenzfahrt_schnittstelle=true;
        delay(3000);                                                // kurze Pause, um die Anzeige zu lesen
        lcd.clear();
        break;
    }
  }

  if (referenzfahrt_anzeige_fertig == true)
  {
    referenzfahrt_anzeige_0 = false;
    referenzfahrt_anzeige_fertig = false; 
    referenzfahrt_done= true;                                      //Sprung zur Auswahl Waagen
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
  
  // ------------------------------------------------------------

  //--------------------------- Koordinaten Anzeigen wenn über Schnittstelle die Befehle ankommen ----------------

  if(serielle_schnittstelle==true && digitalRead(taster_enter)==HIGH)
    {
      lcd.setCursor(0,0);
      lcd.print("Aktuelle Position:");
      lcd.setCursor(0,1);
      lcd.print("X: ");
      long x= ist_position_x / SCHRITT_X;
      lcd.print(x); lcd.print(" mm. ");
      lcd.setCursor(0,2);
      lcd.print("Z: ");
      long z= ist_position_z / SCHRITT_Z;                                
      lcd.print(z); lcd.print(" mm.    ");
      lcd.setCursor(0,3);
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
        lcd.print(ist_position_x);
        x_verfahren(plus, speedMotorX);
      }
      if ((digitalRead(taster_xn) == LOW) && digitalRead(endschalter_xn)==LOW)
      {
        lcd.setCursor(0,3);
        lcd.print("X Position");
        lcd.print(ist_position_x);
        x_verfahren(minus, speedMotorX);
      }
      if ((digitalRead(taster_zp) == LOW) && digitalRead(endschalter_zp)==LOW)
      {
        lcd.setCursor(0,3);
        lcd.print("Z Position");
        lcd.print(ist_position_z);
        z_verfahren(plus, speedMotorZ);

      }
      if ((digitalRead(taster_zn) == LOW) && digitalRead(endschalter_zn)==LOW)
      {
        lcd.setCursor(0,3);
        lcd.print("Z Position");
        lcd.print(ist_position_z);
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
    // ------------------------------------------------------------
    
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
  
  // print the string when a CR arrives:
  if (stringComplete) 
  {

    Serial.println(inputString);
    if (inputString[0] == 'r' || inputString[0] == 'x' || inputString[0] == 'z'
        ||inputString[0] == 'o' ||inputString[0] == 'n' || inputString[0]== 'a' ||inputString[0] == 'h' || inputString[0] == 'v' || inputString[0] == 's')
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

        case 'x': // X-Achse xxxx  xxxx = pos in mm
        {
          Wegx =  ((inputString[1]-'0')*1000) +
                  ((inputString[2]-'0')*100) +
                  ((inputString[3]-'0')*10) +
                  ((inputString[4]-'0'));
          Serial.print("Fahre ");
          Serial.print(Wegx);
          Serial.println(" mm");
          NeuePosx = Wegx * SCHRITT_X;
        
          if ((digitalRead(endschalter_xp)==LOW)&&(digitalRead(endschalter_xn)==LOW) && NeuePosx >=0 && referenzfahrt_done == true && ist_position_z == 0)
          {
            delay(500);
            x_verfahren_position(speedMotorX, NeuePosx);    
            inputString = "";
            Serial3.print("X+\r");
            stringComplete = false;
          }
          
          else if ((digitalRead(endschalter_xp)==LOW)&&(digitalRead(endschalter_xn)==LOW) && ist_position_z > 0)       //Wenn die Maschine nicht auf Z=0, dann fahre ganz nach oben
          {
            delay(500);
            z_verfahren_position(speedMotorZ, 0);
            x_verfahren_position(speedMotorX, NeuePosx);
            inputString = "";
            Serial3.print("X+\r");
            stringComplete = false;
          }
          else
          {
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
          Serial.print("Die Kette hat ");
          Serial.print(AnzSch);
          Serial.println(" Scheiben insgesammt");

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
          long gap_scheiben_eingabe =   ((inputString[1]-'0')*100) +
                                        ((inputString[2]-'0')*10) + 
                                        ((inputString[3]-'0')*1);

          float gap_mm = gap_scheiben_eingabe / 100.0;  // Umwandlung in z.zz Format

          Serial.print("Der Abstand zwischen den Gewichtsgliedern ist ");
          Serial.print(gap_mm, 2);
          Serial.println(" mm gross");
          if(gap_scheiben_eingabe >= 10)
          {
            gap_scheiben_schritte= (gap_scheiben_eingabe* SCHRITT_Z) / 100;
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

        case 'h':
        {
               abstand_boden_gewichtskette =   ((inputString[1]-'0')*1000) +
                                               ((inputString[2]-'0')*100) + 
                                               ((inputString[3]-'0')*10)+
                                               ((inputString[4]-'0')*1);
          Serial.print("Die Gewichtskette ist ");
          Serial.print(abstand_boden_gewichtskette/10);
          Serial.println(" cm vom Boden Entfernt");
          abstand_boden_gewichtskette_schritte= abstand_boden_gewichtskette*SCHRITT_Z;
          long gewichtskette_laenge = z_achse_gesammt - abstand_boden_gewichtskette_schritte;
          Serial.print("1:");
          Serial.println(abstand_boden_gewichtskette_schritte);
          Serial.print("2:");
          Serial.println(gewichtskette_laenge);


          if(abstand_boden_gewichtskette_schritte > gewichtskette_laenge)
          {
            inputString = "";
            Serial3.print("H+\r");
            stringComplete = false;
          }
          else
          {
            inputString = "";
            Serial3.print("H-\r");
            stringComplete = false;
          }

        }
        break;

        case 'o':
        {
          hoehe_waage=  ((inputString[1]-'0')*100) + 
                        ((inputString[2]-'0')*10)+
                        ((inputString[3]-'0')*1);
          Serial.print("Als Hoehe der Waage wurde ");
          Serial.print(hoehe_waage);
          Serial.println(" mm ausgewaeht");
          hoehe_waage_schritte=abstand_boden_gewichtskette_schritte-(hoehe_waage*SCHRITT_Z);
          if(hoehe_waage_schritte< abstand_boden_gewichtskette_schritte )
          {
            position_offset= hoehe_waage_schritte;
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
          auswahl_gewicht = ((inputString[1]-'0')*10) +     //2
                            ((inputString[2]-'0')*1);
          if(ist_position_z<=position_offset)
          {
            z_verfahren_position(speedMotorZ, position_offset);
          }
          
          delay(3000);

          if(auswahl_gewicht<=AnzSch)
          {
            long strecke= gap_scheiben_schritte*auswahl_gewicht;

            Serial.print("Fahre Gewichtskette um: ");
            Serial.print(strecke);
            Serial.println(" mm nach unten");

            z_verfahren_position(speedGewichteAblegen, strecke + position_offset);

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
            digitalWrite(spannungsversorgung24, LOW);            //Ausshalten der Spannungen an den Wägezellen
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
            delay(1000);                          //Ausshalten der Spannungen an den Wägezellen
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
  if (zStart > 0)                         // Z-Achse bewegt
  {
    if ((millis()- zStart)> zBremsWait)   // Z-Achse  seit zBremsWait(ms) nicht bewegt
    {
      digitalWrite(motorbremse, LOW);     // Motorbremse aktivieren
      delay(200);
      digitalWrite(en_z, HIGH);

      zStart = 0;
    }
  }
}