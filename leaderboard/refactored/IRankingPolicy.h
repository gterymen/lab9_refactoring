#pragma once

#include <string>

// Підсумок учасника по всіх заїздах.
struct Standing
{
    std::string name;
    int points;
    int wins;
};

// Правило порівняння учасників у рейтингу (Strategy).
class IRankingPolicy
{
public:
    virtual ~IRankingPolicy() = default;
    virtual bool ranksBefore(Standing const& a, Standing const& b) const = 0;
};

// Поведінка початкового коду: лише бали (рівні бали зберігають алфавітний порядок).
class PointsOnlyPolicy : public IRankingPolicy
{
public:
    bool ranksBefore(Standing const& a, Standing const& b) const override
    {
        return a.points > b.points;
    }
};

// ІНДИВІДУАЛЬНЕ РОЗШИРЕННЯ: за рівних балів вище той, хто має більше перемог.
class WinsTieBreakPolicy : public IRankingPolicy
{
public:
    bool ranksBefore(Standing const& a, Standing const& b) const override
    {
        if (a.points != b.points)
        {
            return a.points > b.points;
        }
        return a.wins > b.wins;
    }
};
