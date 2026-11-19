#pragma once

#include "Driver.h"
#include "IPointsSystem.h"
#include "IRace.h"

#include <list>
#include <memory>
#include <string>
#include <vector>

class Race : public IRace
{
    std::string m_name;
    std::vector<std::shared_ptr<const Driver>> m_results;
    std::shared_ptr<const IPointsSystem> m_points;

public: 
    // Сумісний конструктор початкового коду (копіює об'єкти як Driver).
    Race(std::string const& name, std::list<Driver> const& drivers);

    // Поліморфний конструктор: зберігає справжній тип учасників
    // (SelfDrivingCar тощо) та дозволяє підставити систему балів.
    Race(std::string const& name,
         std::vector<std::shared_ptr<const Driver>> drivers,
         std::shared_ptr<const IPointsSystem> pointsSystem = std::make_shared<DefaultPointsSystem>());

    int position(Driver const& driver) const;

    int getPoints(Driver const& driver) const;

    std::list<Driver> getResults() const;

    std::string getDriverName(Driver const& driver) const;
    
    std::string toString() const;

    std::vector<RaceEntry> entries() const override;
};
