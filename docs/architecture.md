# Architecture

Skillbase follows a layered architecture with a clear separation of concerns between UI, application logic, and data access.

```text
┌─────────────────────────────┐
│           Qt UI             │
│       Qt Widgets            │
│  (UI + application logic    │
│   live in the same layer)   │
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

**UI and application logic (Qt Widgets)**
Responsible for presentation, user interaction, and — in the current state of the project — the domain rules that drive them. Widgets never build SQL directly; they call into the data layer and render whatever it returns.

Keeping UI and application logic in the same layer is a deliberate tradeoff for an application of this size. The logic is small enough that splitting it into a separate service layer would add indirection without a real benefit. If the logic grows (complex scheduling, cross-hobby aggregation, background processing), the natural next step is to extract dedicated service classes between `MainWindow` and the repositories.

**Data layer (repositories)**
One repository per aggregate, each responsible for CRUD operations and query building for its own tables via Qt SQL. Repositories are the only place SQL strings live. They return plain structs (`Hobby`, `Exercise`, `Goal`, `RoutineStep`, `RoadmapStep`, …) defined in their own headers — the UI never builds a query itself.

Current repositories:

| Repository | Owns |
|---|---|
| `HobbyRepository` | `hobbies` |
| `HobbyNoteRepository` | `hobby_notes` |
| `CategoryRepository` | `categories` |
| `ExerciseRepository` | `exercises` |
| `ExerciseLogRepository` | `exercise_logs` |
| `RoutineRepository` | `routines`, `routine_steps`, `routine_logs` |
| `GoalRepository` | `goals` |
| `RoadmapRepository` | `roadmap_steps` |
| `TimelinePhaseRepository` | `timeline_phases` |

**SQLite**
A single local database file. See [`er-diagram.md`](er-diagram.md) for the schema.

## Design decisions

### Why a repository per aggregate instead of one Database class

Early versions of the project accessed the database through a single `Database` class with static methods called directly from the UI. This works for a small prototype but doesn't scale: it's hard to test, hard to mock, and it blurs the line between "what the UI needs" and "how the data is stored".

`Database` is now reduced to connection setup and schema creation only. Data access lives in one repository per aggregate, and each repository owns all SQL for its tables.

### Why SQLite over a file-based format (JSON/CSV)

Skillbase's data is relational by nature (a routine references multiple exercises, an exercise has many logs, a roadmap has parent/child steps) and grows over time. SQLite gives transactional writes, foreign key constraints, and indexed queries for free, without requiring a server process — which fits the "local desktop app, no required cloud services" goal.

### Why a generic `value` + `unit` pair in `exercise_logs` instead of typed columns

Different hobbies track fundamentally different things (BPM, repetitions, minutes, words learned). Rather than creating a wide table with mostly-null typed columns, a single generic numeric value with a unit label keeps the schema simple and hobby-agnostic. The tradeoff: if a single log entry ever needs multiple simultaneous measurements (e.g. weight *and* reps in one set), this will need to be revisited — either with a small `exercise_log_values` child table or additional named columns.

### Why one `is_current` flag per hobby for goals, routines, and roadmaps

The dashboard can only meaningfully show *one* current goal, *one* current roadmap, and up to three current routines per hobby. Instead of a separate "dashboard settings" table, each of those aggregates carries an `is_current` flag on the row itself. The repository enforces "only one per hobby" (or "at most three" for routines) on write.

### Why roadmap steps are self-referential

A roadmap is a tree, not a flat list. Rather than a separate `roadmap_step_parents` join table, `roadmap_steps.parent_id` points at another row in the same table. `parent_id IS NULL` means the step is a root (top-level roadmap). This keeps queries simple (`WHERE parent_id = ?`) at the cost of requiring recursive queries for deep traversal.

### Why dates and timestamps are stored as TEXT

SQLite's `DATE` and `DATETIME` type names are just affinity hints — the values are stored as text anyway. Rather than rely on SQLite's loose date handling, `performed_at`, `deadline`, `start_date`, and `end_date` are stored explicitly as `TEXT` in ISO-8601 form (`yyyy-MM-dd` for dates, `yyyy-MM-dd HH:mm:ss` for timestamps). This makes string sorting equivalent to chronological sorting and lets the UI parse them with `QDate` / `QDateTime` without any conversion layer.

## Adding a new design decision

When a non-obvious architectural choice is made, add a short entry here: what was decided, what the alternative was, and why. Keep entries to a few sentences — this file documents *why*, not a full changelog.