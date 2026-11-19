#include <iostream>

#include "Book.h"
#include "DatabaseBookRepository.h"
#include "Library.h"
#include "Notifiers.h"

int main() {
    DatabaseBookRepository repository;
    EmailNotifier notifier;       // для SMS: SmsNotifier notifier;
    Library library(repository, notifier);

    Book book1("The Catcher in the Rye", "J.D. Salinger");
    Book book2("To Kill a Mockingbird", "Harper Lee");

    library.addBook(book1);
    library.addBook(book2);

    library.displayBooks(std::cout);

    return 0;
}
