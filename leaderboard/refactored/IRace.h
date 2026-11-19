#pragma once

#include <string>
#include <vector>

// Результат одного учасника в одному заїзді.
struct RaceEntry
{
    std::string name;   // ім'я для таблиці результатів
    int position;       // 1 = перемога
    int points;
};

// Абстракція заїзду, від якої залежить Leaderboard (DIP).
// Завдяки їй Leaderboard тестується без реальних Race та Driver.
class IRace
{
public:
    virtual ~IRace() = default;
    virtual std::vector<RaceEntry> entries() const = 0;
};
