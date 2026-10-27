# Capital City Dominoes

A C++20 word-chain game engine for capital cities, with a multi-threaded batch simulator and a heuristic opponent. Built as a university assignment (CO5073) in Visual Studio.

In the game, players take turns naming a city. Each city must start with the last letter of the previous one, no city can be repeated, and the player who cannot move loses.

## Features

- Rule-checked word-chain game engine (`Prototype`) with a seedable random generator
- City list loaded from a plain text file (`Cities.txt`)
- Self-play simulator (`Simulator`) that returns the full word sequence of each game
- Multi-threaded batch simulation: work is split across `std::thread` workers, with per-run seeds derived from one batch seed and results stored behind a mutex
- Heuristic opponent (`Competition`) that picks the city leaving the opponent the fewest valid replies
- Benchmark harness (`Sketchbook.cpp`) comparing `Competition` against the random player over 200 seeded games

## Project structure

```
.
├── 2025ProjectTemplate.sln        Visual Studio solution
├── README.md
├── .gitignore
└── 202425Project/
    ├── 202425project.vcxproj      Project file (C++20, MSVC v143, x64)
    ├── Cities.txt                 City list (one per line)
    ├── Prototype.h / .cpp         Game core
    ├── Simulator.h / .cpp         Multi-threaded simulator
    ├── Competition.h / .cpp       Heuristic opponent
    └── Sketchbook.cpp             Benchmark (Competition vs Prototype)
```

## Requirements

- Windows 10/11, x64
- Visual Studio 2022 or newer with the **Desktop development with C++** workload (MSVC v143 toolset)

## Build and run

1. Open `2025ProjectTemplate.sln` in Visual Studio.
2. Choose the `Sketchbook` configuration and `x64`.
3. Press `Ctrl+F5`.

It prints how many of the 200 games the `Competition` player won.

> The interactive game, test and submission configurations (`Debug`, `Feedback`, `Submit`) depend on files supplied by the university (`202425Project/template/`), which are not included in this repository.

## Copyright

Copyright (c) 2026 Daniyal. All rights reserved.

This is coursework. The course-provided template files are the property of the university and are not part of this repository.
