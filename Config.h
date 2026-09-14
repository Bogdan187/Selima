#pragma once

// -------------------- Richtungen / Schritte --------------------
#define plus 0
#define minus 1

#define SCHRITT_X 160
#define SCHRITT_Z 160
#define maxScheiben 6

// -------------------- Tastenfeld --------------------
constexpr int taster_zp = 46;      // ↑
constexpr int taster_zn = 47;      // ↓
constexpr int taster_xp = 48;      // ←
constexpr int taster_xn = 49;      // →
constexpr int taster_plus = 42;    // +
constexpr int taster_minus = 43;   // -
constexpr int taster_0 = 44;       // 0
constexpr int taster_enter = 45;   // ENT

// -------------------- Endschalter --------------------
constexpr int endschalter_xp = 40;
constexpr int endschalter_xn = 39;
constexpr int endschalter_zp = 51;          //52
constexpr int endschalter_zn = 53;          //53

// -------------------- Motorsteuerkarte X-Achse --------------------
constexpr int dir_x = 9;
constexpr int pul_x = 10;
constexpr int en_x = 11;

// -------------------- Motorsteuerkarte Z-Achse --------------------
constexpr int dir_z = 6;
constexpr int pul_z = 7;
constexpr int en_z = 8;

// -------------------- Motorbremse --------------------
constexpr int motorbremse = 22;

// -------------------- Spannungsversorgung Wägezellen --------------------
constexpr int spannungsversorgung24 = 24;
constexpr int spannungsversorgung12 = 25;

// -------------------- Sichtschutz / Rollos --------------------
constexpr int rollo_up = 26;
constexpr int rollo_down = 27;

// -------------------- Positionen für Gewichte --------------------
constexpr int position_stufe_a = 1140;
constexpr int position_stufe_b = 2480;
constexpr int position_stufe_c = 4180;
constexpr int position_stufe_d = 5380;
constexpr int position_stufe_e = 6580;
constexpr int position_stufe_f = 8580;

// -------------------- Grenzen / Waagenpositionen --------------------
constexpr long grenze_z_kabel = 88000;
constexpr long grenze_z = 98000;
constexpr long grenze_x_tischanfang = 275000;
constexpr long grenze_x_tischende = 291000;

constexpr long position_waage_1 = 275000;
constexpr long position_waage_2 = 291000;
constexpr long position_waage_3 = 286000;
constexpr long position_waage_4 = 286000;
constexpr long position_waage_5 = 286000;
constexpr long position_waage_6 = 286000;
constexpr long position_waage_7 = 286000;
constexpr long position_waage_8 = 286000;
constexpr long position_waage_9 = 286000;
constexpr long position_waage_10 = 286000;
constexpr long position_waage_11 = 286000;
constexpr long position_waage_12 = 286000;

// -------------------- Rampe / Bremse --------------------
constexpr int grenze = 550;
#define zBremsWait 10000
