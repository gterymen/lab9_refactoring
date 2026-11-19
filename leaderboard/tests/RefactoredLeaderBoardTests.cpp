#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "IPointsSystem.h"
#include "IRace.h"
#include "IRankingPolicy.h"
#include "LeaderBoard.h"
#include "Race.h"
#include "SelfDrivingCar.h"

namespace {

using DriverPtr = std::shared_ptr<const Driver>;

DriverPtr driver(const std::string& name, const std::string& country) {
    return std::make_shared<Driver>(name, country);
}

// --- Виправлені дефекти початкового коду ---

TEST(Polymorphism, SelfDrivingCarKeepsItsDisplayName) {
    std::vector<DriverPtr> drivers = {
        std::make_shared<SelfDrivingCar>("1.0", "Tesla"),
        driver("Nico Rosberg", "DE"),
        driver("Lewis Hamilton", "UK")};
    Race race("Australian GP", drivers);
    Leaderboard board(std::vector<std::shared_ptr<const IRace>>{std::make_shared<Race>(race)});

    auto results = board.driverResults();
    EXPECT_EQ(25, results["Self Driving Car - Tesla (1.0)"]);
    EXPECT_EQ(0, results.count("1.0"));
}

TEST(Polymorphism, SelfDrivingCarsOfDifferentCompaniesAreDistinct) {
    std::vector<DriverPtr> drivers = {
        std::make_shared<SelfDrivingCar>("1.0", "Tesla"),
        std::make_shared<SelfDrivingCar>("1.0", "Waymo")};
    Race race("Race", drivers);
    auto entries = race.entries();
    ASSERT_EQ(2u, entries.size());
    EXPECT_EQ("Self Driving Car - Tesla (1.0)", entries[0].name);
    EXPECT_EQ("Self Driving Car - Waymo (1.0)", entries[1].name);
}

TEST(Points, DriversBeyondThirdPlaceGetZeroPoints) {
    std::vector<DriverPtr> drivers = {driver("A", "x"), driver("B", "x"), driver("C", "x"), driver("D", "x")};
    Race race("Race", drivers);
    EXPECT_EQ(0, race.getPoints(Driver("D", "x")));
    EXPECT_EQ(15, race.getPoints(Driver("C", "x")));
}

TEST(Points, DriverNotInRaceGetsZeroPoints) {
    Race race("Race", std::vector<DriverPtr>{driver("A", "x"), driver("B", "x")});
    EXPECT_EQ(0, race.getPoints(Driver("Z", "x")));
}

// --- Ін'єкція системи балів (OCP) ---

class OnePointPerRace : public IPointsSystem {
public:
    int pointsFor(std::size_t position) const override { return position == 0 ? 1 : 0; }
};

TEST(Points, CustomPointsSystemCanBeInjected) {
    Race race("Race", std::vector<DriverPtr>{driver("A", "x"), driver("B", "x")},
              std::make_shared<OnePointPerRace>());
    EXPECT_EQ(1, race.getPoints(Driver("A", "x")));
    EXPECT_EQ(0, race.getPoints(Driver("B", "x")));
}

// --- Незалежне тестування Leaderboard через підроблений IRace (DIP) ---

class FakeRace : public IRace {
public:
    explicit FakeRace(std::vector<RaceEntry> entries) : entries_(std::move(entries)) {}
    std::vector<RaceEntry> entries() const override { return entries_; }
private:
    std::vector<RaceEntry> entries_;
};

std::shared_ptr<const IRace> fake(std::vector<RaceEntry> entries) {
    return std::make_shared<FakeRace>(std::move(entries));
}

TEST(LeaderboardIsolated, SumsPointsFromAnyRaceImplementation) {
    Leaderboard board({fake({{"A", 1, 10}, {"B", 2, 4}}),
                       fake({{"A", 2, 3}, {"B", 1, 7}})});
    auto results = board.driverResults();
    EXPECT_EQ(13, results["A"]);
    EXPECT_EQ(11, results["B"]);
}

TEST(LeaderboardIsolated, RankingUsesTotalPoints) {
    Leaderboard board({fake({{"A", 1, 1}, {"B", 2, 5}, {"C", 3, 3}})});
    std::list<std::string> expected = {"B", "C", "A"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST(LeaderboardIsolated, EmptyRaceListGivesEmptyResults) {
    Leaderboard board(std::vector<std::shared_ptr<const IRace>>{});
    EXPECT_TRUE(board.driverResults().empty());
}

// --- ІНДИВІДУАЛЬНЕ РОЗШИРЕННЯ: тай-брейк за кількістю перемог ---

std::vector<std::shared_ptr<const IRace>> tiedRaces() {
    // Zed: 10 + 1 = 11 балів, 1 перемога; Amy: 4 + 7 = 11 балів, 0 перемог.
    return {fake({{"Zed", 1, 10}, {"Amy", 2, 4}}),
            fake({{"Amy", 2, 7}, {"Zed", 3, 1}})};
}

TEST(TieBreak, DefaultPolicyKeepsAlphabeticalOrderForEqualPoints) {
    Leaderboard board(tiedRaces());
    std::list<std::string> expected = {"Amy", "Zed"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST(TieBreak, MoreWinsRanksHigherWhenPointsAreEqual) {
    Leaderboard board(tiedRaces(), std::make_shared<WinsTieBreakPolicy>());
    std::list<std::string> expected = {"Zed", "Amy"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST(TieBreak, PointsStillHavePriorityOverWins) {
    Leaderboard board({fake({{"A", 1, 1}, {"B", 2, 20}}),
                       fake({{"A", 1, 1}, {"B", 2, 20}})},
                      std::make_shared<WinsTieBreakPolicy>());
    std::list<std::string> expected = {"B", "A"};
    EXPECT_EQ(expected, board.driverRankings());
}

TEST(TieBreak, StandingsContainWinCounts) {
    Leaderboard board(tiedRaces(), std::make_shared<WinsTieBreakPolicy>());
    auto standings = board.standings();
    ASSERT_EQ(2u, standings.size());
    EXPECT_EQ("Zed", standings[0].name);
    EXPECT_EQ(1, standings[0].wins);
    EXPECT_EQ(0, standings[1].wins);
}

TEST(TieBreak, EqualPointsInSingleRaceDecidedByWins) {
    Leaderboard board({fake({{"B", 1, 5}, {"A", 2, 5}})}, std::make_shared<WinsTieBreakPolicy>());
    std::list<std::string> expected = {"B", "A"};
    EXPECT_EQ(expected, board.driverRankings());
}

}  // namespace
