#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

// -------------------- Simulierte Tasten --------------------
extern bool taster_zp_simuliert;
extern bool taster_zn_simuliert;
extern bool taster_xp_simuliert;
extern bool taster_xn_simuliert;
extern bool taster_plus_simuliert;
extern bool taster_minus_simuliert;
extern bool taster_0_simuliert;
extern bool taster_enter_simuliert;

// -------------------- Versorgung / Rollo --------------------
extern int spannung;
extern int zustandRollo;
extern bool endschaltertest_verfahren;
extern bool clear_display;

// -------------------- Rechte Waage --------------------
extern long hoehe_waage_rechts;
extern long x_koordinate_waage_rechts;
extern long abstand_kette_boden;
extern int anzahl_scheiben_rechts;
extern long gap_scheiben_rechts;
extern long position_offset_schritte;

extern long position_z_waage_1;
extern long position_z_waage_2;
extern long position_z_waage_3;
extern long position_z_waage_4;
extern long position_z_waage_5;
extern long position_z_waage_6;
extern long position_z_waage_7;
extern long position_z_waage_8;
extern long position_z_waage_9;
extern long position_z_waage_10;
extern long position_z_waage_11;
extern long position_z_waage_12;

extern long position_waage_rechts;
extern long position_z_waage_rechts_koordinate;
extern long position_x_waage_rechts_koordinate;

// -------------------- Motorgeschwindigkeiten --------------------
extern int speedMotorX;
extern int speedMotorReferenzfahrt_X;
extern int speedMotorReferenzfahrtNegativ_X;
extern int speedMotorZ;
extern int speedMotorReferenzfahrt_Z;
extern int speedMotorReferenzfahrtNegativ_Z;
extern int speedGewichteAblegen;

// -------------------- Status --------------------
extern int z_phase;
extern int x_phase;
extern bool referenzfahrt_anzeige_0;
extern bool referenzfahrt_anzeige_1;
extern bool referenzfahrt_anzeige_fertig;
extern bool referenzfahrt;
extern bool referenzfahrt_x;
extern bool referenzfahrt_z;
extern bool referenzpunkt_x;
extern bool referenzpunkt_z;
extern bool referenzfahrt_z_done;
extern bool manuellerModus;
extern bool referenzfahrt_done;
extern bool funktion_auswahlWaagen;
extern bool auswahlWaagen_status;
extern bool konfiguration_waagen;
extern bool position_verfahren_modus;
extern bool position_erreicht;
extern bool nullpunkt_gesetzt;
extern bool dauerbelastungstest;
extern bool waagen_links_befahren;
extern bool x_referenzfahrt_skip;
extern bool referenzfahrt_schnittstelle;
extern bool konfiguration_waage_rechts_fertig;
extern bool serielle_schnittstelle;

// -------------------- Auswahl Waagen / Gewichtskette --------------------
extern int wahl_gewichtskette;
extern int gewichte_drauf_rechts;
extern double auswahl_gewicht;
extern int anzahl_gewichtsscheiben;
extern int waagennummer;
extern int gewichtskette[];
extern long z_achse_gesammt;
extern long abstand_boden_gewichtskette_schritte;
extern int arrayGroesse;
extern int waageHight[12];
extern int index_waage[12];
extern int maxLast[12];
extern int gewichtOptionen[];
extern int anzahlOptionen;
extern int lastIndex[12];
extern int gewichtOptionen_auswahl;
extern long abstand_boden_gewichtskette_max;

// -------------------- Positionen --------------------
extern long ist_position_x;
extern long ist_position_z;
extern long ist_nullpunkt_z;
extern long position_gewicht;
extern long millimeter_x;
extern long millimeter_z;
extern long Wegx;
extern long NeuePosx;
extern long Posz;
extern long NeuePosz;
extern long Nullz;
extern bool Offsetz;
extern int AnzSch;
extern long AnzSchS;
extern long gap_scheiben_eingabe;
extern double gap_scheiben_schritte;
extern long DiffZ0;
extern long DiffZ0S;
extern long hoehe_waage;
extern long hoehe_waage_schritte;
extern long abstand_boden_gewichtskette;
extern long position_offset;
extern int z_phase_referenzfahrt;

// -------------------- Rampe --------------------
extern float zaehler_rampe;

// -------------------- Serielle Schnittstelle --------------------
extern String inputString;
extern bool stringComplete;
extern bool bMitAnz;
extern bool ReferenzSchnittstelle;

// -------------------- Automatisches Bremsen --------------------
extern unsigned long zStart;
