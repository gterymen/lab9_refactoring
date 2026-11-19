# Початкова версія (як у методичці, з виправленням const) і рефакторизована.
add_executable(LibraryOriginal original/main.cpp)

add_library(LibraryCore STATIC refactored/Library.cpp)
target_include_directories(LibraryCore PUBLIC refactored)

add_executable(LibraryRefactored refactored/main.cpp)
target_link_libraries(LibraryRefactored PRIVATE LibraryCore)

add_executable(LibraryTests tests/LibraryTests.cpp)
target_link_libraries(LibraryTests PRIVATE LibraryCore GTest::gtest GTest::gtest_main)
gtest_discover_tests(LibraryTests)
