#pragma once

#include "Book.h"

// Абстракція механізму сповіщення (DIP, ISP): один вузький інтерфейс.
class INotifier {
public:
    virtual ~INotifier() = default;
    virtual void notifyBookAdded(const Book& book) = 0;
};
