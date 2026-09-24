# Skillbase

**Skillbase** is a local desktop application for organizing and developing personal hobbies and skills over the long term — built with C++ and Qt6.

> Skillbase should always show me what I've achieved, where I currently stand, what I should work on next, and where I want to go long-term — without overloading me with unnecessary features.

## Motivation

Unlike general-purpose tools like Notion or Obsidian, Skillbase doesn't aim to offer as many features as possible. Instead, it's deliberately narrow: everything in the app exists to answer four questions about a skill or hobby the user is developing:

- What have I already achieved?
- Where do I currently stand?
- What should I work on next?
- Where do I want to go long-term?

Skillbase runs entirely locally, with no required cloud services, subscriptions, or AI features.

## Features

| Area | Description |
|---|---|
| **Hobbies** | Any number of hobbies, each individually configurable with name and color |
| **Exercises** | Create, edit, run, and archive exercises; numeric values are tracked over time |
| **Routines** | Combine multiple exercises into a sequential training session |
| **Timer** | Manually controlled timer for exercises and routines with an audio signal on completion |
| **Goals** | Concrete goals with a deadline, progress, and status, linkable to exercises |
| **Progress** | Hobby-specific metrics (e.g. BPM, repetitions, learning level) tracked and visualized over time |
| **History** | Chronological record of past activity |
| **Roadmap** | Planned development path for a hobby |
| **Timeline** | Key events in a hobby's development over time |

## Architecture

Skillbase follows a layered architecture with a clear separation of concerns:

```text
┌─────────────────────────────┐
│           Qt UI             │
│       Qt Widgets            │
└──────────────┬──────────────┘
               ▼
┌─────────────────────────────┐
│      Application Logic      │
│       C++ / OOP             │
└──────────────┬──────────────┘
               ▼
┌─────────────────────────────┐
│        Data Layer           │
│   Repository Pattern /      │
│        Qt SQL               │
└──────────────┬──────────────┘
               ▼
┌─────────────────────────────┐
│          SQLite             │
└─────────────────────────────┘
```

Design decisions and further detail: [`docs/architecture.md`](docs/architecture.md)

## Data model

The full entity-relationship model (hobbies, exercises, routines, goals, logs) lives in [`docs/er-diagram.md`](docs/er-diagram.md).

## Tech stack

- **Language:** C++
- **GUI:** Qt 6, Qt Widgets, Qt Designer
- **Database:** SQLite via Qt SQL
- **Build system:** CMake
- **Tooling:** Qt Creator, Visual Studio Code

## Getting started

### Requirements

- Qt 6 (including the Qt SQL module)
- CMake ≥ 3.16
- A C++17-capable compiler

### Build

```bash
git clone https://github.com/<your-user>/Skillbase.git
cd Skillbase
mkdir build && cd build
cmake ..
cmake --build .
```

On first launch, the application automatically creates a local SQLite database.

## Project structure

```text
Skillbase/
│
├── src/            # Implementation (.cpp)
├── include/        # Headers (.h)
├── resources/      # Qt Designer .ui files
├── tests/          # Tests for core functionality
├── docs/           # Architecture, data model, design decisions
│
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## Project status & roadmap

Skillbase is under active development. For current progress, planned features, and known limitations, see the [Issues](../../issues) and [Projects](../../projects) tabs of this repository rather than this file — that way the README stays accurate without needing constant edits.

## License

This project is licensed under the [MIT License](LICENSE).
