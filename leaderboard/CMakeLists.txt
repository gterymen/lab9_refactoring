# Початкова та рефакторизована версії збираються окремо; спільні тести
# (початковий інтерфейс) компілюються з обома версіями.
add_library(LeaderBoardOriginalLib STATIC
  original/Driver.cpp original/Race.cpp original/LeaderBoard.cpp original/SelfDrivingCar.cpp)
target_include_directories(LeaderBoardOriginalLib PUBLIC original)

add_library(LeaderBoardLib STATIC
  refactored/Driver.cpp refactored/Race.cpp refactored/LeaderBoard.cpp refactored/SelfDrivingCar.cpp)
target_include_directories(LeaderBoardLib PUBLIC refactored)

add_executable(LeaderBoardDemoOriginal demo/Demo.cpp)
target_link_libraries(LeaderBoardDemoOriginal PRIVATE LeaderBoardOriginalLib)

add_executable(LeaderBoardDemoRefactored demo/Demo.cpp)
target_link_libraries(LeaderBoardDemoRefactored PRIVATE LeaderBoardLib)

add_executable(LeaderBoardTestsOriginal tests/LeaderBoardTests.cpp)
target_link_libraries(LeaderBoardTestsOriginal PRIVATE LeaderBoardOriginalLib GTest::gtest GTest::gtest_main)
gtest_discover_tests(LeaderBoardTestsOriginal TEST_PREFIX original.)

add_executable(LeaderBoardTestsRefactored tests/LeaderBoardTests.cpp tests/RefactoredLeaderBoardTests.cpp)
target_link_libraries(LeaderBoardTestsRefactored PRIVATE LeaderBoardLib GTest::gtest GTest::gtest_main)
gtest_discover_tests(LeaderBoardTestsRefactored TEST_PREFIX refactored.)
