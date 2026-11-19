#pragma once

#include <ostream>

#include "Book.h"

// Відображення винесено з моделі (SRP): Book більше не знає про std::cout.
inline void printBook(const Book& book, std::ostream& out) {
    out << "Title: " << book.title()
        << ", Author: " << book.author() << std::endl;
}
