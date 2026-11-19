# Лабораторна робота 9. Рефакторинг та принципи SOLID

Студент: Стасишін Олександр, група ФІТ 3-11.

Проєкт містить два завдання:

1. library/ - обов'язкове: рефакторинг системи бібліотеки (Book, Library).
2. leaderboard/ - Kata "LeaderBoard" (Racing-Car-Katas) та індивідуальне розширення (тай-брейк за кількістю перемог).

У кожній частині є папка original/ (початковий код) і refactored/ (після рефакторингу).

## Структура

```
library/
  original/main.cpp          початковий код з методички
  refactored/                Book, IBookRepository, INotifier, Library, ...
  tests/LibraryTests.cpp     тести з тестовими дублями
leaderboard/
  original/                  початковий Driver, SelfDrivingCar, Race, Leaderboard
  refactored/                IPointsSystem, IRace, IRankingPolicy, Race, Leaderboard, ...
  demo/Demo.cpp              еталонний прогін (golden master)
  tests/                     LeaderBoardTests (обидві версії) і RefactoredLeaderBoardTests
```

## Збірка та запуск

Потрібні Visual Studio (x64 Native Tools Command Prompt) і vcpkg:

```
set VCPKG_ROOT=C:\vcpkg
cmake --preset default
cmake --build build --config Release
ctest --test-dir build -C Release
```

Очікуваний результат: 100% tests passed out of 46.

Порівняння поведінки до і після рефакторингу:

```
build\library\LibraryOriginal.exe > a.txt
build\library\LibraryRefactored.exe > b.txt
fc a.txt b.txt
build\leaderboard\LeaderBoardDemoOriginal.exe > c.txt
build\leaderboard\LeaderBoardDemoRefactored.exe > d.txt
fc c.txt d.txt
```

В обох випадках: FC: no differences encountered.

## Висновки по SOLID (коротко)

- SRP: Book лише зберігає дані; збереження, сповіщення й відображення винесені в окремі класи.
- OCP: нові сховища, способи сповіщення, системи балів і правила рейтингу додаються новими класами (IBookRepository, INotifier, IPointsSystem, IRankingPolicy), без змін наявного коду.
- LSP: SelfDrivingCar тепер коректно підставляється замість Driver (віртуальний getDisplayName замість typeid, зберігання через вказівники виключає "зрізання").
- ISP: інтерфейси вузькі, по одному-два методи.
- DIP: Library і Leaderboard залежать від абстракцій (INotifier, IBookRepository, IRace), реалізації передаються через конструктор.
