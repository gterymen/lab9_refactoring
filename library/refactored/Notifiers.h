#pragma once

#include <iostream>

#include "INotifier.h"

class EmailNotifier : public INotifier {
public:
    void notifyBookAdded(const Book&) override {
        std::cout << "Sending email notification about the new book..."
                  << std::endl;
    }
};

// Друга реалізація: показує, що спосіб сповіщення замінюється без змін у Library.
class SmsNotifier : public INotifier {
public:
    void notifyBookAdded(const Book&) override {
        std::cout << "Sending SMS notification about the new book..."
                  << std::endl;
    }
};
