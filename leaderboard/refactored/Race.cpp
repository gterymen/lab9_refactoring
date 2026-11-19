#include "Race.h"

Race::Race(std::string const& name, std::list<Driver> const& drivers)
    : m_name(name), m_points(std::make_shared<DefaultPointsSystem>())
{
    for (Driver const& driver : drivers)
    {
        m_results.push_back(std::make_shared<Driver>(driver));
    }
}

Race::Race(std::string const& name,
           std::vector<std::shared_ptr<const Driver>> drivers,
           std::shared_ptr<const IPointsSystem> pointsSystem)
    : m_name(name), m_results(std::move(drivers)), m_points(std::move(pointsSystem))
{}

int Race::position(Driver const& driver) const
{
    int count = 0;
    for (auto const& d : m_results)
    {
        if (*d == driver)
        {
            break;
        }
        ++count;
    }
    return count;
}

int Race::getPoints(Driver const& driver) const
{
    int index = position(driver);
    if (index >= static_cast<int>(m_results.size()))
    {
        return 0;   // учасника немає в заїзді (раніше: вихід за межі масиву)
    }
    return m_points->pointsFor(static_cast<std::size_t>(index));
}

std::list<Driver> Race::getResults() const
{
    std::list<Driver> results;
    for (auto const& d : m_results)
    {
        results.push_back(*d);
    }
    return results;
}

std::string Race::getDriverName(Driver const& driver) const
{
    for (auto const& d : m_results)
    {
        if (*d == driver)
        {
            return d->getDisplayName();
        }
    }
    return "";
}

std::string Race::toString() const
{
    return m_name;
}

std::vector<RaceEntry> Race::entries() const
{
    std::vector<RaceEntry> result;
    for (std::size_t i = 0; i < m_results.size(); ++i)
    {
        result.push_back({ m_results[i]->getDisplayName(),
                           static_cast<int>(i) + 1,
                           m_points->pointsFor(i) });
    }
    return result;
}
