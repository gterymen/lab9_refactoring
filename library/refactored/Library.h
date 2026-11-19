#pragma once

#include <cstddef>
#include <ostream>
#include <vector>

#include "Book.h"
#include "IBookRepository.h"
#include "INotifier.h"

// Бізнес-логіка бібліотеки. Залежить лише від абстракцій; конкретні
// реалізації передаються ззовні через конструктор (Dependency Injection).
class Library {
public:
    Library(IBookRepository& repository, INotifier& notifier);

    void addBook(const Book& book);
    void displayBooks(std::ostream& out) const;
    std::size_t size() const { return books_.size(); }

private:
    std::vector<Book> books_;
    IBookRepository& repository_;
    INotifier& notifier_;
};
