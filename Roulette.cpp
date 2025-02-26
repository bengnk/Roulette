#include "Roulette.hpp"

RouletteColor getRouletteColor(int number) {
    if (number == 0)
        return RC_Green;
    int redNumbers[18] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    for (int i = 0; i < 18; ++i) {
        if (redNumbers[i] == number)
            return RC_Red;
    }
    return RC_Black;
}

int getBallNumber(float ballAngle, float wheelAngle) {
    float adjustedAngle = std::fmod(ballAngle + 90 + wheelAngle, 360.0f);
    if (adjustedAngle < 0)
        adjustedAngle += 360.0f;
    int sectorIndex = static_cast<int>(adjustedAngle / SECTOR_ANGLE) % NUM_SECTORS;
    return ROULETTE_NUMBERS[sectorIndex];
}
