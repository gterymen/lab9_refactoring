#pragma once

#include "Book.h"

// Абстракція сховища книг (DIP): Library не знає, куди саме зберігаються дані.
class IBookRepository {
public:
    virtual ~IBookRepository() = default;
    virtual void save(const Book& book) = 0;
};
