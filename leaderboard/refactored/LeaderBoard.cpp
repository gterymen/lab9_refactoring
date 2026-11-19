#include "LeaderBoard.h"

#include <algorithm>

Leaderboard::Leaderboard(std::list<Race> const& races)
    : m_policy(std::make_shared<PointsOnlyPolicy>())
{
    for (Race const& race : races)
    {
        m_races.push_back(std::make_shared<Race>(race));
    }
}

Leaderboard::Leaderboard(std::vector<std::shared_ptr<const IRace>> races,
                         std::shared_ptr<const IRankingPolicy> policy)
    : m_races(std::move(races)), m_policy(std::move(policy))
{}

std::map<std::string, int> Leaderboard::driverResults() const
{
    std::map<std::string, int> results;
    for (auto const& race : m_races)
    {
        for (RaceEntry const& entry : race->entries())
        {
            results[entry.name] += entry.points;
        }
    }
    return results;
}

std::vector<Standing> Leaderboard::standings() const
{
    std::map<std::string, Standing> byName;   // алфавітний порядок як початковий
    for (auto const& race : m_races)
    {
        for (RaceEntry const& entry : race->entries())
        {
            Standing& s = byName.try_emplace(entry.name, Standing{ entry.name, 0, 0 }).first->second;
            s.points += entry.points;
            if (entry.position == 1)
            {
                s.wins += 1;
            }
        }
    }

    std::vector<Standing> result;
    for (auto const& item : byName)
    {
        result.push_back(item.second);
    }
    std::stable_sort(result.begin(), result.end(), [this](Standing const& a, Standing const& b)
    {
        return m_policy->ranksBefore(a, b);
    });
    return result;
}

std::list<std::string> Leaderboard::driverRankings() const
{
    std::list<std::string> names;
    for (Standing const& s : standings())
    {
        names.push_back(s.name);
    }
    return names;
}
