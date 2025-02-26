#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

// -------------------- Konstanten und Typdefinitionen --------------------

// Physikalische Parameter für das Roulette-Rad
const int NUM_SECTORS = 37;                // Zahlen 0 bis 36
const float SECTOR_ANGLE = 360.0f / NUM_SECTORS;
const float WHEEL_RADIUS = 250.0f;           // Für die Anzeige der Nummern am Rad
const float BALL_CIRCLE_RADIUS = 115.0f;     // Bahn, auf der der Ball rotiert

// Reihenfolge der Zahlen auf einem europäischen Roulette-Rad:
const int ROULETTE_NUMBERS[NUM_SECTORS] = {
        0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24,
        16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26
};

// Wettarten:
enum BetType { NumberBet, ColorBet, DozenBet };

// Zur Kennzeichnung der Farben (Roulette hat Rot, Schwarz und die 0 in Grün):
enum RouletteColor { RC_Green, RC_Red, RC_Black };

// -------------------- Funktionserklärungen --------------------

/// Ermittelt die Farbe einer Zahl (0 = Grün; ansonsten je nach Roulette‑Standard)
RouletteColor getRouletteColor(int number);

/// Ermittelt anhand des Ball- und Radwinkels die Gewinnzahl.
/// Wir nehmen an, dass bei einem ungedrehten Rad die 0 oben liegt.
int getBallNumber(float ballAngle, float wheelAngle);

#endif // ROULETTE_HPP
