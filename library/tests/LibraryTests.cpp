#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

#include "Library.h"

namespace {

// Тестові дублі: записують виклики замість реальної роботи (стає можливим
// тестувати Library ізольовано завдяки абстракціям).
class FakeRepository : public IBookRepository {
public:
    void save(const Book& book) override {
        events->push_back("save:" + book.title());
    }
    std::vector<std::string>* events = nullptr;
};

class FakeNotifier : public INotifier {
public:
    void notifyBookAdded(const Book& book) override {
        events->push_back("notify:" + book.title());
    }
    std::vector<std::string>* events = nullptr;
};

struct Fixture : ::testing::Test {
    std::vector<std::string> events;
    FakeRepository repository;
    FakeNotifier notifier;
    void SetUp() override {
        repository.events = &events;
        notifier.events = &events;
    }
};

TEST(BookTest, GettersReturnConstructorValues) {
    Book book("Dune", "Frank Herbert");
    EXPECT_EQ("Dune", book.title());
    EXPECT_EQ("Frank Herbert", book.author());
}

TEST_F(Fixture, AddBookSavesAndNotifies) {
    Library library(repository, notifier);
    library.addBook(Book("Dune", "Frank Herbert"));
    ASSERT_EQ(2u, events.size());
    EXPECT_EQ("save:Dune", events[0]);
    EXPECT_EQ("notify:Dune", events[1]);
}

TEST_F(Fixture, SaveHappensBeforeNotify) {
    Library library(repository, notifier);
    library.addBook(Book("A", "x"));
    library.addBook(Book("B", "y"));
    EXPECT_EQ((std::vector<std::string>{"save:A", "notify:A", "save:B", "notify:B"}), events);
}

TEST_F(Fixture, SizeGrowsWithEveryBook) {
    Library library(repository, notifier);
    EXPECT_EQ(0u, library.size());
    library.addBook(Book("A", "x"));
    library.addBook(Book("B", "y"));
    EXPECT_EQ(2u, library.size());
}

TEST_F(Fixture, DisplayBooksPrintsInInsertionOrder) {
    Library library(repository, notifier);
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger"));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee"));
    std::ostringstream out;
    library.displayBooks(out);
    EXPECT_EQ("Title: The Catcher in the Rye, Author: J.D. Salinger\n"
              "Title: To Kill a Mockingbird, Author: Harper Lee\n",
              out.str());
}

TEST_F(Fixture, DisplayOfEmptyLibraryPrintsNothing) {
    Library library(repository, notifier);
    std::ostringstream out;
    library.displayBooks(out);
    EXPECT_TRUE(out.str().empty());
}

// Заміна реалізацій без зміни Library: інші класи, той самий Library.
class CountingNotifier : public INotifier {
public:
    void notifyBookAdded(const Book&) override { ++count; }
    int count = 0;
};

class MemoryRepository : public IBookRepository {
public:
    void save(const Book& book) override { titles.push_back(book.title()); }
    std::vector<std::string> titles;
};

TEST(Replaceability, LibraryWorksWithOtherRepositoryAndNotifier) {
    MemoryRepository repository;
    CountingNotifier notifier;
    Library library(repository, notifier);
    library.addBook(Book("A", "x"));
    library.addBook(Book("B", "y"));
    EXPECT_EQ((std::vector<std::string>{"A", "B"}), repository.titles);
    EXPECT_EQ(2, notifier.count);
}

}  // namespace
