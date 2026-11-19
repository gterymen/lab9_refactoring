#pragma once

#include <string>
#include <utility>

// Модель даних: лише зберігає назву й автора (інкапсуляція через private + getters).
class Book {
public:
    Book(std::string title, std::string author)
        : title_(std::move(title)), author_(std::move(author)) {}

    const std::string& title() const { return title_; }
    const std::string& author() const { return author_; }

private:
    std::string title_;
    std::string author_;
};
