#include "Library.h"

#include "BookPrinter.h"

Library::Library(IBookRepository& repository, INotifier& notifier)
    : repository_(repository), notifier_(notifier) {}

void Library::addBook(const Book& book) {
    books_.push_back(book);
    repository_.save(book);
    notifier_.notifyBookAdded(book);
}

void Library::displayBooks(std::ostream& out) const {
    for (const auto& book : books_) {
        printBook(book, out);
    }
}
