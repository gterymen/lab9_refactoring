#pragma once

#include <iostream>

#include "IBookRepository.h"

// Імітація збереження в базу даних (поведінка збігається з початковим кодом).
class DatabaseBookRepository : public IBookRepository {
public:
    void save(const Book&) override {
        std::cout << "Saving book to database..." << std::endl;
    }
};
