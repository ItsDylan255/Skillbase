# Data model

Entity-relationship diagram for Skillbase's core tables. Render this on GitHub automatically, or paste the code block into the [Mermaid Live Editor](https://mermaid.live).

```mermaid
erDiagram
  HOBBIES ||--o{ EXERCISES : has
  HOBBIES ||--o{ ROUTINES : has
  HOBBIES ||--o{ GOALS : has
  EXERCISES ||--o{ EXERCISE_LOGS : has
  EXERCISES ||--o{ ROUTINE_STEPS : used_in
  ROUTINES ||--o{ ROUTINE_STEPS : contains
  ROUTINES ||--o{ ROUTINE_LOGS : has

  HOBBIES {
    int id PK
    string name
    string color
    datetime created_at
  }
  EXERCISES {
    int id PK
    int hobby_id FK
    string name
    string description
    string category
    string goal
    bool archived
  }
  EXERCISE_LOGS {
    int id PK
    int exercise_id FK
    datetime performed_at
    float value
    string unit
    int duration_seconds
  }
  ROUTINES {
    int id PK
    int hobby_id FK
    string name
    string description
  }
  ROUTINE_STEPS {
    int id PK
    int routine_id FK
    int exercise_id FK
    int position
    int duration_seconds
  }
  ROUTINE_LOGS {
    int id PK
    int routine_id FK
    datetime performed_at
    int duration_seconds
  }
  GOALS {
    int id PK
    int hobby_id FK
    string title
    date deadline
    int progress_percent
    string status
  }
```

## Notes

- `routine_steps` is the join table between `routines` and `exercises`. It carries its own `position` (order within the routine) and `duration_seconds` (how long the exercise runs in *this* routine, independent of the exercise itself).
- `exercise_logs` uses a generic `value` + `unit` pair rather than named columns, so the same table works for BPM, minutes, repetitions, etc. This is a deliberate simplification: it doesn't support logging multiple simultaneous measurements (e.g. weight *and* reps in one set). Revisit with a child table (`exercise_log_values`) only if that's actually needed.
- `category` currently lives as a plain text field on `exercises` rather than its own table. Promote it to a `categories` table only if categories need to be managed independently (renamed, reused across hobbies, etc.).
- Hobbies are identified by name and color only — no icon field. Custom hobby icons were considered and deliberately dropped from scope.
- Milestones were considered as a separate table but deliberately dropped from scope; achieved goals are tracked via `goals.status`.
