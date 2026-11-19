#pragma once

#include "IRace.h"
#include "IRankingPolicy.h"
#include "Race.h"

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Leaderboard
{
    std::vector<std::shared_ptr<const IRace>> m_races;
    std::shared_ptr<const IRankingPolicy> m_policy;

public:
    // Сумісний конструктор початкового коду.
    Leaderboard(std::list<Race> const& races);

    // Залежить від абстракцій IRace та IRankingPolicy.
    Leaderboard(std::vector<std::shared_ptr<const IRace>> races,
                std::shared_ptr<const IRankingPolicy> policy = std::make_shared<PointsOnlyPolicy>());

    std::map<std::string, int> driverResults() const;

    std::list<std::string> driverRankings() const;

    std::vector<Standing> standings() const;
};
