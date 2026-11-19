#include <iostream>
#include <list>

#include "LeaderBoard.h"
#include "Race.h"

// Еталонний прогін (golden master) через початковий інтерфейс: однаковий
// для початкової та рефакторизованої версій.
int main()
{
    Driver nico("Nico Rosberg", "DE");
    Driver lewis("Lewis Hamilton", "UK");
    Driver sebastian("Sebastian Vettel", "DE");
    Driver kimi("Kimi Raikkonen", "FI");

    Race r1("Australian GP", {nico, lewis, sebastian});
    Race r2("Malaysian GP", {sebastian, lewis, nico});
    Race r3("Chinese GP", {lewis, nico, sebastian});
    Race r4("Bahrain GP", {kimi, nico, lewis});

    Leaderboard board({r1, r2, r3, r4});

    std::cout << "Driver results:\n";
    for (const auto& entry : board.driverResults())
    {
        std::cout << "  " << entry.first << ": " << entry.second << "\n";
    }

    std::cout << "Driver rankings:\n";
    int place = 1;
    for (const auto& name : board.driverRankings())
    {
        std::cout << "  " << place++ << ". " << name << "\n";
    }
    return 0;
}
