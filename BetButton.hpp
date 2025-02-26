#ifndef BETBUTTON_HPP
#define BETBUTTON_HPP

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Roulette.hpp"  // Enthält u.a. BetType und RouletteColor

/// Struktur für einen Wett-Button – verwendbar für Zahlen-, Farb- oder Dutzendwetten
struct BetButton {
    BetType betType;
    int number = 0;               // Nur bei NumberBet (z. B. 0 oder 1–36)
    RouletteColor betColor;       // Nur bei ColorBet (RC_Red oder RC_Black)
    int dozenIndex = 0;           // Nur bei DozenBet: 1 für 1.-Dutzend, 2 für 2.-, 3 für 3.-Dutzend
    sf::RectangleShape shape;
    sf::Text text;
    int betAmount = 0;
};

#endif // BETBUTTON_HPP
