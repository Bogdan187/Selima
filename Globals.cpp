#include "Globals.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

// -------------------- Simulierte Tasten --------------------
bool taster_zp_simuliert = false;
bool taster_zn_simuliert = false;
bool taster_xp_simuliert = false;
bool taster_xn_simuliert = false;
bool taster_plus_simuliert = false;
bool taster_minus_simuliert = false;
bool taster_0_simuliert = false;
bool taster_enter_simuliert = false;

// -------------------- Versorgung / Rollo --------------------
int spannung = 0;
int zustandRollo = 0;
bool endschaltertest_verfahren = false;
bool clear_display = false;


// -------------------- Rechte Waage --------------------
long hoehe_waage_rechts = 100;
long x_koordinate_waage_rechts = 1000;
long abstand_kette_boden = 500;
int anzahl_scheiben_rechts = 4;
long gap_scheiben_rechts = 8.0;
long abstand_boden_gewichtskette_max=200;

long position_z_waage_1 = 0;
long position_z_waage_2 = 0;
long position_z_waage_3 = 0;
long position_z_waage_4 = 0;
long position_z_waage_5 = 0;
long position_z_waage_6 = 0;
long position_z_waage_7 = 0;
long position_z_waage_8 = 0;
long position_z_waage_9 = 0;
long position_z_waage_10 = 0;
long position_z_waage_11 = 0;
long position_z_waage_12 = 0;

long position_waage_rechts = 100;
long position_z_waage_rechts_koordinate = 0;
long position_x_waage_rechts_koordinate = 0;

// -------------------- Motorgeschwindigkeiten --------------------
int speedMotorX = 700;
int speedMotorReferenzfahrt_X = 1200;
int speedMotorReferenzfahrtNegativ_X = 700;
int speedMotorZ = 700;
int speedMotorReferenzfahrt_Z = 700;
int speedMotorReferenzfahrtNegativ_Z = 1200;
int speedGewichteAblegen = 1000;

// -------------------- Status --------------------
int z_phase = 0;
int x_phase = 0;
bool referenzfahrt_anzeige_0 = true;
bool referenzfahrt_anzeige_1 = false;
bool referenzfahrt_anzeige_fertig = false;
bool referenzfahrt = true;
bool referenzfahrt_x = false;
bool referenzfahrt_z = false;
bool referenzpunkt_x = false;
bool referenzpunkt_z = false;
bool referenzfahrt_z_done = false;
bool manuellerModus = false;
bool referenzfahrt_done = false;
bool funktion_auswahlWaagen = false;
bool auswahlWaagen_status = false;
bool konfiguration_waagen = false;
bool position_verfahren_modus = false;
bool position_erreicht = false;
bool nullpunkt_gesetzt = false;
bool dauerbelastungstest = false;
bool waagen_links_befahren = false;
bool x_referenzfahrt_skip = false;
bool referenzfahrt_schnittstelle = false;
bool konfiguration_waage_rechts_fertig = false;
bool serielle_schnittstelle = false;

// -------------------- Auswahl Waagen / Gewichtskette --------------------
int wahl_gewichtskette = 0;
int gewichte_drauf_rechts = 1;
double auswahl_gewicht = 0;
int anzahl_gewichtsscheiben = 6;
int waagennummer = 0;
int gewichtskette[] = {2, 2, 1, 3, 5, 5};
long z_achse_gesammt = 144000;
long abstand_boden_gewichtskette_schritte = 0;
int arrayGroesse = sizeof(gewichtskette) / sizeof(gewichtskette[0]);
int waageHight[12] = {100,100,100,100,100,100,100,100,100,100,100,100};
int index_waage[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int maxLast[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int gewichtOptionen[] = {2, 4, 5, 8, 13, 18, 0};
int anzahlOptionen = 6;
int lastIndex[12] = {0};
int gewichtOptionen_auswahl = 0;

// -------------------- Positionen --------------------
long ist_position_x = 0.0;
long ist_position_z = 0.0;
long ist_nullpunkt_z = 0.0;
long position_gewicht = 0;
long millimeter_x = 0;
long millimeter_z = 0;
long Wegx = 0;
long NeuePosx = 0;
long Posz = 0;
long NeuePosz = 0;
long Nullz = 0;
bool Offsetz = false;
int AnzSch = 0;
long AnzSchS = 0;
long gap_scheiben_eingabe = 0;
double gap_scheiben_schritte = 0.0;
long DiffZ0 = 330;
long DiffZ0S = 0;
long hoehe_waage = 0;
long hoehe_waage_schritte = 0;
long abstand_boden_gewichtskette = 0;
long position_offset = 0;
long position_offset_schritte = 0;
int z_phase_referenzfahrt=0;

// -------------------- Rampe --------------------
float zaehler_rampe = 0.0;

// -------------------- Serielle Schnittstelle --------------------
String inputString = "";
bool stringComplete = false;
bool bMitAnz = false;
bool ReferenzSchnittstelle = false;

// -------------------- Automatisches Bremsen --------------------
unsigned long zStart = 0;
