#pragma once

#include <cstddef>

// Абстракція системи нарахування балів (OCP): нові системи додаються
// новими класами, а Race не змінюється.
class IPointsSystem
{
public:
    virtual ~IPointsSystem() = default;
    virtual int pointsFor(std::size_t zeroBasedPosition) const = 0;
};

// Система початкового коду: 25, 18, 15; решта учасників балів не отримує.
class DefaultPointsSystem : public IPointsSystem
{
public:
    int pointsFor(std::size_t zeroBasedPosition) const override
    {
        static const int kPoints[] = { 25, 18, 15 };
        return zeroBasedPosition < 3 ? kPoints[zeroBasedPosition] : 0;
    }
};
