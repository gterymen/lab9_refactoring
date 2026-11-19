#include <gtest/gtest.h>

#include <list>
#include <map>
#include <string>

#include "LeaderBoard.h"
#include "Race.h"
#include "SelfDrivingCar.h"

// Ці тести використовують лише ПОЧАТКОВИЙ інтерфейс (Race(name, list<Driver>),
// Leaderboard(list<Race>)), тому компілюються й з початковою, й з рефакторизованою версією.
namespace {

struct Drivers : ::testing::Test {
    Driver nico{"Nico Rosberg", "DE"};
    Driver lewis{"Lewis Hamilton", "UK"};
    Driver sebastian{"Sebastian Vettel", "DE"};
};

TEST_F(Drivers, DriversAreEqualWhenNameAndCountryMatch) {
    EXPECT_TRUE(nico == Driver("Nico Rosberg", "DE"));
    EXPECT_FALSE(nico == Driver("Nico Rosberg", "UK"));
    EXPECT_FALSE(nico == lewis);
}

TEST_F(Drivers, PositionIsZeroBased) {
    Race race("Australian GP", {nico, lewis, sebastian});
    EXPECT_EQ(0, race.position(nico));
    EXPECT_EQ(1, race.position(lewis));
    EXPECT_EQ(2, race.position(sebastian));
}

TEST_F(Drivers, PointsAre25_18_15) {
    Race race("Australian GP", {nico, lewis, sebastian});
    EXPECT_EQ(25, race.getPoints(nico));
    EXPECT_EQ(18, race.getPoints(lewis));
    EXPECT_EQ(15, race.getPoints(sebastian));
}

TEST_F(Drivers, RaceKeepsOrderOfResults) {
    Race race("Australian GP", {lewis, nico, sebastian});
    std::list<Driver> results = race.getResults();
    ASSERT_EQ(3u, results.size());
    EXPECT_EQ("Lewis Hamilton", results.front().getName());
    EXPECT_EQ("Sebastian Vettel", results.back().getName());
}

TEST_F(Drivers, RaceToStringReturnsName) {
    Race race("Australian GP", {nico, lewis, sebastian});
    EXPECT_EQ("Australian GP", race.toString());
}

TEST_F(Drivers, DriverNameInRaceIsPlainName) {
    Race race("Australian GP", {nico, lewis, sebastian});
    EXPECT_EQ("Nico Rosberg", race.getDriverName(nico));
}

TEST_F(Drivers, ResultsSumPointsOverRaces) {
    Race r1("Australian GP", {nico, lewis, sebastian});
    Race r2("Malaysian GP", {sebastian, lewis, nico});
    Race r3("Chinese GP", {lewis, nico, sebastian});
    Leaderboard board({r1, r2, r3});

    std::map<std::string, int> results = board.driverResults();
    EXPECT_EQ(3u, results.size());
    EXPECT_EQ(58, results["Nico Rosberg"]);
    EXPECT_EQ(61, results["Lewis Hamilton"]);
    EXPECT_EQ(55, results["Sebastian Vettel"]);
}

TEST_F(Drivers, RankingsAreOrderedByPointsDescending) {
    Race r1("Australian GP", {nico, lewis, sebastian});
    Race r2("Malaysian GP", {sebastian, lewis, nico});
    Race r3("Chinese GP", {lewis, nico, sebastian});
    Leaderboard board({r1, r2, r3});

    std::list<std::string> rankings = board.driverRankings();
    std::list<std::string> expected = {"Lewis Hamilton", "Nico Rosberg", "Sebastian Vettel"};
    EXPECT_EQ(expected, rankings);
}

TEST_F(Drivers, SingleRaceRankingFollowsFinishOrder) {
    Race race("Australian GP", {sebastian, nico, lewis});
    Leaderboard board({race});
    std::list<std::string> expected = {"Sebastian Vettel", "Nico Rosberg", "Lewis Hamilton"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST_F(Drivers, EqualPointsKeepAlphabeticalOrder) {
    Race r1("A", {nico, lewis, sebastian});
    Race r2("B", {lewis, nico, sebastian});
    Leaderboard board({r1, r2});   // Nico та Lewis мають по 43 бали
    std::list<std::string> expected = {"Lewis Hamilton", "Nico Rosberg", "Sebastian Vettel"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST(LeaderboardEmpty, NoRacesGiveNoResults) {
    Leaderboard board(std::list<Race>{});
    EXPECT_TRUE(board.driverResults().empty());
    EXPECT_TRUE(board.driverRankings().empty());
}

// Автомобіль без водія. При передачі за значенням у std::list<Driver> об'єкт
// "зрізається" до Driver, тому в таблиці з'являється версія алгоритму як ім'я
// (поведінка початкового коду; зафіксована як характеристичний тест).
TEST_F(Drivers, SelfDrivingCarPassedByValueIsSlicedToDriver) {
    SelfDrivingCar car("1.0", "Tesla");
    Race race("Australian GP", {car, nico, lewis});
    Leaderboard board({race});

    std::map<std::string, int> results = board.driverResults();
    EXPECT_EQ(25, results["1.0"]);
    EXPECT_EQ(18, results["Nico Rosberg"]);
    EXPECT_EQ(15, results["Lewis Hamilton"]);
}

TEST(SelfDrivingCarTest, StoresAlgorithmVersionAndCompany) {
    SelfDrivingCar car("1.0", "Tesla");
    EXPECT_EQ("1.0", car.getAlgorithmVersion());
    EXPECT_EQ("Tesla", car.getCountry());
    car.setAlgorithmVersion("2.0");
    EXPECT_EQ("2.0", car.getAlgorithmVersion());
}

}  // namespace
