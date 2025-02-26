#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Roulette.hpp"
#include "BetButton.hpp"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // -------------------- Vollbildfenster erstellen --------------------
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Roulette Simulation", sf::Style::Fullscreen);
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.f;
    float centerY = windowSize.y / 2.f;

    // -------------------- Hintergrundbild laden --------------------
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/Administrator/OneDrive - HWR Berlin/Dokumente/HWR/Roulett/background.jpeg")) {
        std::cerr << "Fehler beim Laden des Hintergrundbildes!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    // Skaliere das Bild, damit es den gesamten Bildschirm füllt:
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // -------------------- Ressourcen laden --------------------
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture wheelTexture;
    if (!wheelTexture.loadFromFile("C:/Users/Administrator/OneDrive - HWR Berlin/Dokumente/HWR/Roulett/roulette_wheel.png")) {
        std::cerr << "Fehler beim Laden des Roulette-Rads!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite wheelSprite;
    wheelSprite.setTexture(wheelTexture);
    wheelSprite.setOrigin(wheelTexture.getSize().x / 2.0f, wheelTexture.getSize().y / 2.0f);
    wheelSprite.setPosition(centerX, centerY);
    wheelSprite.setScale(0.5f, 0.5f);

    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("C:/Users/Administrator/OneDrive - HWR Berlin/Dokumente/HWR/Roulett/casino_bg.png")) {
        std::cerr << "Fehler beim Laden des Balls!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);
    // Reduziere den Skalierungsfaktor, damit der Ball kleiner erscheint:
    float ballScale = 0.008f;
    ballSprite.setScale(ballScale, ballScale);
    ballSprite.setOrigin(ballTexture.getSize().x / 2.0f, ballTexture.getSize().y / 2.0f);

    // Anzeige der Nummern am Rad (rotierend mit dem Rad)
    std::vector<sf::Text> wheelNumberTexts;
    for (int i = 0; i < NUM_SECTORS; ++i) {
        sf::Text t;
        t.setFont(font);
        t.setString(std::to_string(ROULETTE_NUMBERS[i]));
        t.setCharacterSize(14);
        t.setFillColor(sf::Color::White);
        t.setOrigin(7, 7);
        wheelNumberTexts.push_back(t);
    }

    // -------------------- Spielvariablen --------------------
    float wheelRotationSpeed = 0, ballRotationSpeed = 0;
    float ballAngle = 0.0f;   // in Grad
    float wheelAngle = -6.0f;  // in Grad
    bool spinning = false;

    int balance = 1000;            // Startguthaben
    const int BET_INCREMENT = 10;  // Fester Einsatzbetrag pro Klick

    // -------------------- UI-Layout Parameter --------------------
    const float TABLE_MARGIN_LEFT = 50.0f;
    const float NUMBER_GRID_TOP = 550.0f;
    const float ZERO_BUTTON_WIDTH = 60.0f;
    const float ZERO_BUTTON_HEIGHT = 65.0f;
    const float NUM_BUTTON_WIDTH = 60.0f;
    const float NUM_BUTTON_HEIGHT = 30.0f;
    const float NUM_BUTTON_SPACING = 5.0f;
    float gridXStart = TABLE_MARGIN_LEFT + ZERO_BUTTON_WIDTH + 10.0f;
    std::vector<BetButton> betButtons;

    // ----- 1. Zahlen-Button für "0" -----
    {
        BetButton btn;
        btn.betType = NumberBet;
        btn.number = 0;
        btn.shape.setSize(sf::Vector2f(ZERO_BUTTON_WIDTH, ZERO_BUTTON_HEIGHT));
        btn.shape.setFillColor(sf::Color(0, 150, 0));
        btn.shape.setOutlineColor(sf::Color::Black);
        btn.shape.setOutlineThickness(2);
        btn.shape.setPosition(TABLE_MARGIN_LEFT, NUMBER_GRID_TOP);

        btn.text.setFont(font);
        btn.text.setCharacterSize(14);
        btn.text.setFillColor(sf::Color::White);
        std::stringstream ss;
        ss << "0\n" << btn.betAmount;
        btn.text.setString(ss.str());
        btn.text.setPosition(TABLE_MARGIN_LEFT + 5, NUMBER_GRID_TOP + 5);
        betButtons.push_back(btn);
    }

    // ----- 2. Zahlen-Buttons für 1 bis 36 in einem 3-Spalten/12-Zeilen-Grid -----
    for (int i = 0; i < 12; ++i) {
        // Linke Spalte: Zahlen 3, 6, 9, ... ,36
        {
            BetButton btn;
            btn.betType = NumberBet;
            btn.number = 3 + 3 * i;
            if (getRouletteColor(btn.number) == RC_Red) {
                btn.shape.setFillColor(sf::Color(255, 120, 120));
                btn.text.setFillColor(sf::Color::Black);
            } else {
                btn.shape.setFillColor(sf::Color(80, 80, 80));
                btn.text.setFillColor(sf::Color::White);
            }
            btn.shape.setSize(sf::Vector2f(NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT));
            btn.shape.setOutlineColor(sf::Color::Black);
            btn.shape.setOutlineThickness(2);
            btn.shape.setPosition(gridXStart, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING));

            btn.text.setFont(font);
            btn.text.setCharacterSize(14);
            std::stringstream ss;
            ss << btn.number << "\n" << btn.betAmount;
            btn.text.setString(ss.str());
            btn.text.setPosition(gridXStart + 5, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING) + 3);
            betButtons.push_back(btn);
        }
        // Mittlere Spalte: Zahlen 2, 5, 8, ... ,35
        {
            BetButton btn;
            btn.betType = NumberBet;
            btn.number = 2 + 3 * i;
            if (getRouletteColor(btn.number) == RC_Red) {
                btn.shape.setFillColor(sf::Color(255, 120, 120));
                btn.text.setFillColor(sf::Color::Black);
            } else {
                btn.shape.setFillColor(sf::Color(80, 80, 80));
                btn.text.setFillColor(sf::Color::White);
            }
            btn.shape.setSize(sf::Vector2f(NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT));
            btn.shape.setOutlineColor(sf::Color::Black);
            btn.shape.setOutlineThickness(2);
            float x = gridXStart + NUM_BUTTON_WIDTH + NUM_BUTTON_SPACING;
            btn.shape.setPosition(x, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING));

            btn.text.setFont(font);
            btn.text.setCharacterSize(14);
            std::stringstream ss;
            ss << btn.number << "\n" << btn.betAmount;
            btn.text.setString(ss.str());
            btn.text.setPosition(x + 5, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING) + 3);
            betButtons.push_back(btn);
        }
        // Rechte Spalte: Zahlen 1, 4, 7, ... ,34
        {
            BetButton btn;
            btn.betType = NumberBet;
            btn.number = 1 + 3 * i;
            if (getRouletteColor(btn.number) == RC_Red) {
                btn.shape.setFillColor(sf::Color(255, 120, 120));
                btn.text.setFillColor(sf::Color::Black);
            } else {
                btn.shape.setFillColor(sf::Color(80, 80, 80));
                btn.text.setFillColor(sf::Color::White);
            }
            btn.shape.setSize(sf::Vector2f(NUM_BUTTON_WIDTH, NUM_BUTTON_HEIGHT));
            btn.shape.setOutlineColor(sf::Color::Black);
            btn.shape.setOutlineThickness(2);
            float x = gridXStart + 2 * (NUM_BUTTON_WIDTH + NUM_BUTTON_SPACING);
            btn.shape.setPosition(x, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING));

            btn.text.setFont(font);
            btn.text.setCharacterSize(14);
            std::stringstream ss;
            ss << btn.number << "\n" << btn.betAmount;
            btn.text.setString(ss.str());
            btn.text.setPosition(x + 5, NUMBER_GRID_TOP + i * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING) + 3);
            betButtons.push_back(btn);
        }
    }

    // ----- 3. Dutzend-Wetten (unter dem Zahlen-Grid) -----
    float DOZEN_ROW_Y = NUMBER_GRID_TOP + 12 * (NUM_BUTTON_HEIGHT + NUM_BUTTON_SPACING) + 10;
    const float DOZEN_BUTTON_WIDTH = 140.0f;
    const float DOZEN_BUTTON_HEIGHT = 50.0f;
    const float DOZEN_BUTTON_SPACING = 10.0f;
    for (int i = 1; i <= 3; ++i) {
        BetButton btn;
        btn.betType = DozenBet;
        btn.dozenIndex = i;
        btn.shape.setSize(sf::Vector2f(DOZEN_BUTTON_WIDTH, DOZEN_BUTTON_HEIGHT));
        btn.shape.setFillColor(sf::Color(200, 200, 250));
        btn.shape.setOutlineColor(sf::Color::Black);
        btn.shape.setOutlineThickness(2);
        float x = TABLE_MARGIN_LEFT + (i - 1) * (DOZEN_BUTTON_WIDTH + DOZEN_BUTTON_SPACING);
        btn.shape.setPosition(x, DOZEN_ROW_Y);

        btn.text.setFont(font);
        btn.text.setCharacterSize(14);
        btn.text.setFillColor(sf::Color::Black);
        std::stringstream ss;
        ss << i << ". Dutzend" << "\n" << btn.betAmount;
        btn.text.setString(ss.str());
        btn.text.setPosition(x + 10, DOZEN_ROW_Y + 10);
        betButtons.push_back(btn);
    }

    // ----- 4. Farbwetten: Rot und Schwarz -----
    float COLOR_ROW_Y = DOZEN_ROW_Y + DOZEN_BUTTON_HEIGHT + 10;
    const float COLOR_BUTTON_WIDTH = 140.0f;
    const float COLOR_BUTTON_HEIGHT = 50.0f;
    {
        // Button für "Rot"
        BetButton btn;
        btn.betType = ColorBet;
        btn.betColor = RC_Red;
        btn.shape.setSize(sf::Vector2f(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT));
        btn.shape.setFillColor(sf::Color(255, 150, 150));
        btn.shape.setOutlineColor(sf::Color::Black);
        btn.shape.setOutlineThickness(2);
        btn.shape.setPosition(TABLE_MARGIN_LEFT, COLOR_ROW_Y);

        btn.text.setFont(font);
        btn.text.setCharacterSize(14);
        btn.text.setFillColor(sf::Color::Black);
        std::stringstream ss;
        ss << "Rot" << "\n" << btn.betAmount;
        btn.text.setString(ss.str());
        btn.text.setPosition(TABLE_MARGIN_LEFT + 10, COLOR_ROW_Y + 10);
        betButtons.push_back(btn);
    }
    {
        // Button für "Schwarz"
        BetButton btn;
        btn.betType = ColorBet;
        btn.betColor = RC_Black;
        btn.shape.setSize(sf::Vector2f(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT));
        btn.shape.setFillColor(sf::Color(100, 100, 100));
        btn.shape.setOutlineColor(sf::Color::Black);
        btn.shape.setOutlineThickness(2);
        btn.shape.setPosition(TABLE_MARGIN_LEFT + COLOR_BUTTON_WIDTH + DOZEN_BUTTON_SPACING, COLOR_ROW_Y);

        btn.text.setFont(font);
        btn.text.setCharacterSize(14);
        btn.text.setFillColor(sf::Color::White);
        std::stringstream ss;
        ss << "Schwarz" << "\n" << btn.betAmount;
        btn.text.setString(ss.str());
        btn.text.setPosition(TABLE_MARGIN_LEFT + COLOR_BUTTON_WIDTH + DOZEN_BUTTON_SPACING + 10, COLOR_ROW_Y + 10);
        betButtons.push_back(btn);
    }

    // ----- 5. Hintergrund für den Wettbereich (grüner Filztisch) -----
    sf::RectangleShape bettingAreaBackground;
    float bgX = 30.0f;
    float bgY = NUMBER_GRID_TOP - 20;
    float bgWidth = 520.0f;
    float bgHeight = (COLOR_ROW_Y + COLOR_BUTTON_HEIGHT + 20) - bgY;
    bettingAreaBackground.setPosition(bgX, bgY);
    bettingAreaBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
    bettingAreaBackground.setFillColor(sf::Color(0, 100, 0, 200));
    bettingAreaBackground.setOutlineColor(sf::Color::Black);
    bettingAreaBackground.setOutlineThickness(3);

    // ----- 6. Spin-Button, Balance- und Ergebnisanzeige -----
    sf::RectangleShape spinButton;
    const float SPIN_BUTTON_WIDTH = 120.0f;
    const float SPIN_BUTTON_HEIGHT = 60.0f;
    spinButton.setSize(sf::Vector2f(SPIN_BUTTON_WIDTH, SPIN_BUTTON_HEIGHT));
    spinButton.setFillColor(sf::Color(100, 200, 100));
    spinButton.setOutlineColor(sf::Color::Black);
    spinButton.setOutlineThickness(2);
    spinButton.setPosition(windowSize.x - 320, 600);

    sf::Text spinButtonText;
    spinButtonText.setFont(font);
    spinButtonText.setString("Spin");
    spinButtonText.setCharacterSize(20);
    spinButtonText.setFillColor(sf::Color::Black);
    spinButtonText.setPosition(spinButton.getPosition().x + 25, spinButton.getPosition().y + 15);

    sf::Text balanceText;
    balanceText.setFont(font);
    balanceText.setCharacterSize(20);
    balanceText.setFillColor(sf::Color::White);
    balanceText.setPosition(windowSize.x - 320, spinButton.getPosition().y - 50);
    balanceText.setString("Balance: " + std::to_string(balance));

    sf::RectangleShape resultBackground;
    resultBackground.setSize(sf::Vector2f(320, 180));
    resultBackground.setFillColor(sf::Color(50, 50, 50, 200));
    resultBackground.setOutlineColor(sf::Color::Yellow);
    resultBackground.setOutlineThickness(3);
    resultBackground.setPosition(windowSize.x - 370, spinButton.getPosition().y + SPIN_BUTTON_HEIGHT + 20);

    sf::Text resultText;
    resultText.setFont(font);
    resultText.setCharacterSize(18);
    resultText.setFillColor(sf::Color::Yellow);
    resultText.setPosition(resultBackground.getPosition().x + 10, resultBackground.getPosition().y + 10);
    resultText.setString("");

    // ----- Neuer Exit-Button zum Verlassen des Spiels -----
    sf::RectangleShape exitButton;
    exitButton.setSize(sf::Vector2f(100.0f, 40.0f));
    exitButton.setFillColor(sf::Color(200, 100, 100));
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setOutlineThickness(2);
    exitButton.setPosition(windowSize.x - 120, 20);

    sf::Text exitButtonText;
    exitButtonText.setFont(font);
    exitButtonText.setString("Exit");
    exitButtonText.setCharacterSize(18);
    exitButtonText.setFillColor(sf::Color::Black);
    exitButtonText.setPosition(exitButton.getPosition().x + 20, exitButton.getPosition().y + 10);

    // -------------------- Hauptschleife --------------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                // Überprüfe, ob der Exit-Button geklickt wurde
                if (exitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
                    // Reagiere nur, wenn nicht gedreht wird
                else if (!spinning) {
                    if (spinButton.getGlobalBounds().contains(mousePos)) {
                        int totalBet = 0;
                        for (const auto &btn : betButtons)
                            totalBet += btn.betAmount;
                        if (totalBet > 0) {
                            spinning = true;
                            resultText.setString("");
                            wheelRotationSpeed = static_cast<float>(std::rand() % 3 + 1);
                            ballRotationSpeed  = static_cast<float>(std::rand() % 3 + 2);
                        }
                    } else {
                        for (auto &btn : betButtons) {
                            if (btn.shape.getGlobalBounds().contains(mousePos)) {
                                if (balance >= BET_INCREMENT) {
                                    balance -= BET_INCREMENT;
                                    btn.betAmount += BET_INCREMENT;
                                    std::stringstream ss;
                                    if (btn.betType == NumberBet)
                                        ss << btn.number << "\n" << btn.betAmount;
                                    else if (btn.betType == ColorBet) {
                                        if (btn.betColor == RC_Red)
                                            ss << "Rot" << "\n" << btn.betAmount;
                                        else if (btn.betColor == RC_Black)
                                            ss << "Schwarz" << "\n" << btn.betAmount;
                                    } else if (btn.betType == DozenBet) {
                                        ss << btn.dozenIndex << ". Dutzend" << "\n" << btn.betAmount;
                                    }
                                    btn.text.setString(ss.str());
                                    balanceText.setString("Balance: " + std::to_string(balance));
                                }
                            }
                        }
                    }
                }
            }
        } // Ende Event-Schleife

        // Visuelle Hervorhebung der Buttons
        for (auto &btn : betButtons) {
            if (btn.betAmount > 0) {
                btn.shape.setOutlineThickness(4);
                btn.shape.setOutlineColor(sf::Color::Yellow);
            } else {
                btn.shape.setOutlineThickness(2);
                btn.shape.setOutlineColor(sf::Color::Black);
            }
        }

        // -------------------- Dreh-Animation --------------------
        if (spinning) {
            wheelSprite.rotate(-wheelRotationSpeed);
            wheelAngle += wheelRotationSpeed;
            ballAngle += ballRotationSpeed;
            float rad = ballAngle * 3.14159265f / 180.0f;
            ballSprite.setPosition(centerX + BALL_CIRCLE_RADIUS * std::cos(rad),
                                   centerY + BALL_CIRCLE_RADIUS * std::sin(rad));

            wheelRotationSpeed *= 0.9995f;
            ballRotationSpeed  *= 0.9995f;
            if (wheelRotationSpeed < 0.01f && ballRotationSpeed < 0.01f) {
                spinning = false;
                int winningNumber = getBallNumber(ballAngle, wheelAngle);
                RouletteColor winningColor = getRouletteColor(winningNumber);
                std::string colorStr;
                if (winningColor == RC_Green)
                    colorStr = "Grün";
                else if (winningColor == RC_Red)
                    colorStr = "Rot";
                else if (winningColor == RC_Black)
                    colorStr = "Schwarz";

                std::stringstream resultMsg;
                resultMsg << "Gewinnzahl: " << winningNumber << " (" << colorStr << ")\n\n";
                bool anyWin = false;
                for (auto &btn : betButtons) {
                    int winAmount = 0;
                    if (btn.betType == NumberBet) {
                        if (btn.number == winningNumber) {
                            winAmount = btn.betAmount * 36;
                            resultMsg << "Zahl " << btn.number << ": Einsatz " << btn.betAmount
                                      << " -> Gewinn " << winAmount << "\n";
                            anyWin = true;
                        }
                    } else if (btn.betType == ColorBet) {
                        if (winningNumber != 0 && getRouletteColor(winningNumber) == btn.betColor) {
                            winAmount = btn.betAmount * 2;
                            if (btn.betColor == RC_Red)
                                resultMsg << "Rot: Einsatz " << btn.betAmount << " -> Gewinn " << winAmount << "\n";
                            else if (btn.betColor == RC_Black)
                                resultMsg << "Schwarz: Einsatz " << btn.betAmount << " -> Gewinn " << winAmount << "\n";
                            anyWin = true;
                        }
                    } else if (btn.betType == DozenBet) {
                        bool wins = false;
                        if (winningNumber != 0) {
                            if (btn.dozenIndex == 1 && winningNumber >= 1 && winningNumber <= 12)
                                wins = true;
                            else if (btn.dozenIndex == 2 && winningNumber >= 13 && winningNumber <= 24)
                                wins = true;
                            else if (btn.dozenIndex == 3 && winningNumber >= 25 && winningNumber <= 36)
                                wins = true;
                        }
                        if (wins) {
                            winAmount = btn.betAmount * 3;
                            resultMsg << btn.dozenIndex << ". Dutzend: Einsatz " << btn.betAmount
                                      << " -> Gewinn " << winAmount << "\n";
                            anyWin = true;
                        }
                    }
                    balance += winAmount;
                    btn.betAmount = 0;
                    std::stringstream ss;
                    if (btn.betType == NumberBet)
                        ss << btn.number << "\n" << btn.betAmount;
                    else if (btn.betType == ColorBet) {
                        if (btn.betColor == RC_Red)
                            ss << "Rot" << "\n" << btn.betAmount;
                        else if (btn.betColor == RC_Black)
                            ss << "Schwarz" << "\n" << btn.betAmount;
                    } else if (btn.betType == DozenBet)
                        ss << btn.dozenIndex << ". Dutzend" << "\n" << btn.betAmount;
                    btn.text.setString(ss.str());
                }
                if (!anyWin)
                    resultMsg << "\nKeine Gewinne.";
                resultText.setString(resultMsg.str());
                balanceText.setString("Balance: " + std::to_string(balance));
            }
        }

        // -------------------- Aktualisiere die Anzeige der Nummern am Rad --------------------
        for (int i = 0; i < NUM_SECTORS; ++i) {
            float angle = (i * SECTOR_ANGLE - 90 - wheelAngle) * 3.14159265f / 180.0f;
            float textX = centerX + (WHEEL_RADIUS - 40) * std::cos(angle);
            float textY = centerY + (WHEEL_RADIUS - 40) * std::sin(angle);
            wheelNumberTexts[i].setPosition(textX, textY);
            wheelNumberTexts[i].setRotation(i * SECTOR_ANGLE - wheelAngle);
        }

        // -------------------- Zeichnen --------------------
        window.clear();
        window.draw(backgroundSprite);
        window.draw(wheelSprite);
        for (const auto &t : wheelNumberTexts)
            window.draw(t);
        window.draw(ballSprite);
        window.draw(bettingAreaBackground);
        for (const auto &btn : betButtons) {
            window.draw(btn.shape);
            window.draw(btn.text);
        }
        window.draw(spinButton);
        window.draw(spinButtonText);
        window.draw(balanceText);
        window.draw(resultBackground);
        window.draw(resultText);
        window.draw(exitButton);
        window.draw(exitButtonText);
        window.display();
    }
    return EXIT_SUCCESS;
}
