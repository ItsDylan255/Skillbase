# Architecture

Skillbase follows a layered architecture with a clear separation of concerns between UI, application logic, and data access.

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

## Layers

**UI (Qt Widgets)**
Responsible only for presentation and user interaction. Widgets should not contain business logic or direct SQL calls — they call into the application logic layer and render whatever it returns.

**Application logic**
Owns the domain rules: what counts as a valid exercise, how a routine run progresses, how progress values are aggregated. This layer is plain C++ and doesn't depend on Qt Widgets, which keeps it testable without a GUI.

**Data layer (repositories)**
One repository per aggregate (e.g. `HobbyRepository`, `ExerciseRepository`, `RoutineRepository`), each responsible for CRUD operations and query building for its own tables via Qt SQL. Repositories are the only place SQL strings live.

**SQLite**
A single local database file. See [`er-diagram.md`](er-diagram.md) for the schema.

## Design decisions

### Why a repository per aggregate instead of one Database class

Early versions of the project accessed the database through a single `Database` class with static methods called directly from the UI. This works for a small prototype but doesn't scale: it's hard to test, hard to mock, and it blurs the line between "what the UI needs" and "how the data is stored".

`Database` is now reduced to connection setup and schema creation only. Data access lives in one repository per aggregate (`HobbyRepository`, `ExerciseRepository`, with `RoutineRepository`, `GoalRepository`, etc. to follow as those tables are added). Each repository owns all SQL for its table and returns plain structs (`Hobby`, `Exercise`) defined in their own headers — the UI never builds a query itself.

### Why SQLite over a file-based format (JSON/CSV)

Skillbase's data is relational by nature (a routine references multiple exercises, an exercise has many logs) and grows over time. SQLite gives transactional writes, foreign key constraints, and indexed queries for free, without requiring a server process — which fits the "local desktop app, no required cloud services" goal.

### Why a generic `value` + `unit` pair in `exercise_logs` instead of typed columns

Different hobbies track fundamentally different things (BPM, repetitions, minutes, words learned). Rather than creating a wide table with mostly-null typed columns, a single generic numeric value with a unit label keeps the schema simple and hobby-agnostic. The tradeoff: if a single log entry ever needs multiple simultaneous measurements (e.g. weight *and* reps in one set), this will need to be revisited — either with a small `exercise_log_values` child table or additional named columns.

## Adding a new design decision

When a non-obvious architectural choice is made, add a short entry here: what was decided, what the alternative was, and why. Keep entries to a few sentences — this file documents *why*, not a full changelog.
