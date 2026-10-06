# Skillbase UI Guidelines

This document defines how the Skillbase user interface should be structured and designed.

The guidelines build upon the Skillbase Design System and provide practical rules for navigation, layouts, forms, actions, content, and interaction patterns.

The goal is to make every part of Skillbase feel like the same application.

---

# 1. General Guidelines

Every screen should answer three questions quickly:

1. **Where am I?**
2. **What can I do here?**
3. **What information is important?**

The interface should avoid unnecessary complexity.

Prefer:

* clear hierarchy
* predictable navigation
* consistent layouts
* meaningful labels
* progressive disclosure
* reusable components

Avoid:

* unnecessary screens
* redundant controls
* hidden important actions
* inconsistent terminology
* excessive dialogs
* decorative UI without a purpose

---

# 2. Page Structure

Most Skillbase pages should follow a consistent structure.

```text
Page
├── Header
│   ├── Title
│   ├── Optional description
│   └── Actions
│
└── Content
```

The header establishes the context of the page.

The content contains the actual information and interactions.

---

# 3. Page Headers

Every major page should have a clear title.

Example:

```text
Exercises

Manage and track the exercises for this hobby.

                         [ Add Exercise ]
```

The primary action should normally be located in the page header when it applies to the entire page.

Do not repeat the same action throughout the page without a reason.

---

# 4. Navigation Structure

Skillbase uses hierarchical navigation.

There are two primary levels:

```text
Application
└── Hobby
    └── Hobby Section
```

For example:

```text
Skillbase
└── Guitar
    ├── Dashboard
    ├── Routines
    ├── Exercises
    ├── History
    ├── Progress
    ├── Goals
    ├── Timeline
    └── Roadmap
```

Navigation should communicate the current location clearly.

---

# 5. Application Navigation

Application-level navigation should contain only global areas.

Examples:

```text
Overview
Statistics
Settings
```

Do not place hobby-specific actions in the global sidebar.

The sidebar should remain stable while navigating between hobbies.

---

# 6. Hobby Navigation

Once a hobby is selected, the interface should provide navigation specific to that hobby.

Example:

```text
Guitar

Dashboard
Routines
Exercises
History
Progress
Goals
Timeline
Roadmap
```

The current section must be visually identifiable.

Navigation labels should remain stable throughout the application.

---

# 7. Navigation Labels

Use short, descriptive labels.

Prefer:

```text
Exercises
Progress
History
Goals
```

Avoid:

```text
Manage Your Exercises
Your Personal Progress Overview
Exercise History & Statistics
```

Navigation is not the place for long descriptions.

---

# 8. Dashboard Layout

Dashboards should provide a concise overview rather than attempting to display everything.

A dashboard should prioritize:

1. current status
2. important actions
3. recent activity
4. relevant progress
5. useful secondary information

A possible structure:

```text
Dashboard
│
├── Overview
│   ├── Today
│   ├── Goals
│   └── Progress
│
├── Recent Activity
│
└── Notes
```

Cards should be arranged in a grid when appropriate.

Do not force every card into a single vertical column.

---

# 9. Responsive Card Layout

Cards should use the available desktop space efficiently.

For example:

```text
┌──────────────┐ ┌──────────────┐ ┌──────────────┐
│ Today        │ │ Goals        │ │ Progress     │
│              │ │              │ │              │
└──────────────┘ └──────────────┘ └──────────────┘

┌───────────────────────────────┐
│ Recent Activity               │
└───────────────────────────────┘
```

The exact arrangement may change depending on window size.

Cards should not unnecessarily occupy the full width.

---

# 10. Information Hierarchy

Not all information has equal importance.

Use visual hierarchy to distinguish:

### Primary information

Examples:

* exercise name
* goal progress
* current value
* page title

### Secondary information

Examples:

* category
* last performed
* description
* metadata

### Tertiary information

Examples:

* timestamps
* technical metadata
* supplementary details

Primary information should be visible first.

Secondary and tertiary information should support it without competing for attention.

---

# 11. Progressive Disclosure

Do not show every possible detail immediately.

Show the most relevant information first.

Additional information can be available through:

* details views
* expandable sections
* dialogs
* secondary actions

Example:

```text
Exercise
Pull-ups

Strength · Upper Body
Last performed: Yesterday

[ View Details ]
```

Detailed history does not need to be displayed directly on every overview screen.

---

# 12. Actions

Actions should appear where the user expects them.

### Page-level actions

Place near the page title.

Example:

```text
Exercises                         [ Add Exercise ]
```

### Item-level actions

Place near the item they affect.

Example:

```text
Pull-ups                         [ Edit ] [ ... ]
```

### Destructive actions

Keep destructive actions visually and spatially separated from normal actions where possible.

---

# 13. Primary Action

Each context should generally have one visually dominant primary action.

Examples:

```text
Exercises
                         [ Add Exercise ]
```

```text
Exercise Editor
                         [ Save ]
```

Avoid presenting multiple competing primary actions.

---

# 14. Forms

Forms should follow a predictable structure.

```text
Label
Input

Label
Input

Label
Input

                 [ Cancel ] [ Save ]
```

Related fields should be grouped.

For example:

```text
Exercise Information

Name
Category
Description

Tracking

Unit
Start Value
Goal Value
```

---

# 15. Form Labels

Labels should clearly describe the expected input.

Prefer:

```text
Name
Category
Unit
Goal
```

Avoid vague labels such as:

```text
Value
Data
Information
Option
```

unless the context makes their meaning unambiguous.

---

# 16. Required and Optional Fields

Do not make users guess whether a field is required.

Required fields should be clear through the form design.

Optional information should not clutter the interface with unnecessary indicators.

If most fields are required, it may be preferable to indicate only optional fields.

---

# 17. Input Validation

Validation should happen as close to the input as practical.

Errors should:

* explain what is wrong
* identify the affected field
* provide a clear way to correct it

Avoid generic messages such as:

```text
Invalid input.
```

Prefer:

```text
Goal must be greater than the current value.
```

---

# 18. Confirmation Dialogs

Do not ask for confirmation for every action.

Confirmation is most appropriate when an action:

* deletes data
* permanently changes data
* cannot easily be undone
* has significant consequences

For simple reversible actions, immediate execution is preferable.

---

# 19. Destructive Actions

Destructive actions should be clearly identifiable.

Examples:

```text
Delete Exercise
Delete Hobby
Remove Routine
```

A destructive confirmation dialog should explain what will happen.

Example:

```text
Delete Exercise?

This will permanently delete "Pull-ups".

[ Cancel ] [ Delete ]
```

The destructive action should not be accidentally triggered by the default keyboard action.

---

# 20. Empty States

Empty states should be useful.

Instead of:

```text
No data.
```

provide:

```text
No exercises yet

Create your first exercise to start tracking your progress.

[ Add Exercise ]
```

An empty state should generally contain:

1. what is missing
2. why it matters, when useful
3. what the user can do next

---

# 21. Loading States

Long-running operations should provide feedback.

The interface should not appear frozen.

Depending on the operation, use:

* progress indicators
* loading states
* disabled actions
* status text

Avoid unnecessary loading indicators for operations that complete immediately.

---

# 22. Error States

Errors should be understandable and actionable.

Prefer:

```text
Could not save the exercise.

Check the highlighted fields and try again.
```

over:

```text
Database error.
```

Technical information may be available for debugging without overwhelming the normal user interface.

---

# 23. Success Feedback

Successful actions should provide appropriate feedback.

For minor actions, a subtle state change may be enough.

For important actions, use a clear confirmation.

Avoid excessive success messages for every small interaction.

---

# 24. Tables

Tables should be used when users need to compare multiple records.

Examples:

* exercise history
* routine logs
* progress records
* activity history

A table should prioritize:

* readable columns
* consistent alignment
* clear headers
* useful sorting
* reasonable row height

Avoid displaying every possible database field.

---

# 25. Lists

Lists are appropriate when items are primarily consumed individually.

Examples:

* hobbies
* exercises
* routines
* notes

List items should clearly expose the item's identity.

Example:

```text
Pull-ups
Strength · Upper Body
Last performed yesterday
```

The item should not require unnecessary interaction just to understand what it represents.

---

# 26. Cards vs Lists vs Tables

Use the appropriate representation.

### Cards

Use for:

* summaries
* dashboards
* important metrics
* grouped information

### Lists

Use for:

* collections
* navigation
* individually focused items

### Tables

Use for:

* comparison
* history
* structured datasets
* many records

Do not use cards simply because they look visually attractive.

---

# 27. Tags and Metadata

Metadata should remain visually secondary.

Example:

```text
Pull-ups

[ Strength ] [ Upper Body ]

Last performed
Yesterday
```

Tags should help scanning rather than dominate the item.

---

# 28. Search

Search should be introduced when a collection becomes large enough that scrolling is inefficient.

Search fields should:

* clearly indicate what is searchable
* update results predictably
* provide a way to clear the query

Do not add search fields to small collections where they provide no meaningful benefit.

---

# 29. Filtering

Filtering should be used when users need to reduce a larger dataset.

Examples:

```text
Category
Status
Archived
Date
```

Filters should be easy to understand and remove.

Active filters should be visually identifiable.

---

# 30. Sorting

Sorting should be available where ordering provides meaningful value.

Examples:

```text
Name
Last performed
Created
Progress
```

The current sort order should be clear.

---

# 31. Archive vs Delete

Where appropriate, distinguish between:

```text
Archive
```

and:

```text
Delete
```

Archive should be used when an item may still have historical value.

Delete should be reserved for data that should actually be removed.

Archived items should generally be hidden from normal views unless the user explicitly chooses to display them.

---

# 32. Progress

Progress should be represented in ways that are easy to interpret.

Possible representations include:

* percentages
* progress bars
* current vs target values
* trends
* historical charts

Do not use a progress visualization when the underlying data does not meaningfully represent progress.

---

# 33. Numbers and Units

Numeric values should always provide sufficient context.

Avoid:

```text
25
```

Prefer:

```text
25 kg
```

or:

```text
25 reps
```

The unit should be visually secondary but immediately understandable.

---

# 34. Dates and Times

Dates and times should be presented consistently throughout Skillbase.

Use human-readable formats where possible.

Examples:

```text
Today
Yesterday
Last week
24 Sep 2026
```

Use exact timestamps when precision matters.

---

# 35. Keyboard Interaction

Skillbase is a desktop application and should support keyboard interaction wherever practical.

Important requirements:

* logical tab order
* visible keyboard focus
* standard shortcuts where appropriate
* Enter for expected confirmation actions
* Escape for dismissing dialogs where appropriate

Keyboard interaction should not conflict with normal text input.

---

# 36. Accessibility

The interface should remain usable without relying solely on color.

Do not communicate important information using color alone.

For example, an error should not be represented only by red.

Use:

* text
* icons
* labels
* state indicators

in addition to color.

Interactive elements should have sufficient size and clear states.

---

# 37. Desktop Window Behavior

Skillbase should behave like a desktop application.

Windows should have:

* sensible minimum sizes
* resizable layouts
* appropriate scroll behavior
* stable navigation

Avoid designing the entire interface around one fixed window size.

---

# 38. Dialog vs Page

Use a **page** when the user is working with a substantial amount of information.

Use a **dialog** when the user is performing a focused task.

### Page

Good for:

* exercise management
* history
* progress
* goals
* routines

### Dialog

Good for:

* creating a simple item
* editing a small amount of information
* confirmation
* focused settings

Do not put an entire complex workflow into a small dialog.

---

# 39. Settings

Settings should be organized by purpose.

Avoid one large collection of unrelated controls.

Possible groups:

```text
Appearance
Data
Behavior
Application
```

Each setting should have a clear label and, where necessary, supporting information.

---

# 40. Consistency Rules

When implementing a new screen, reuse existing patterns whenever possible.

For example:

If an existing page uses:

```text
Title + Description + Primary Action
```

a new page should use the same structure when its purpose is similar.

If an existing form uses a specific field layout, new forms should follow the same pattern.

Consistency is preferred over introducing a visually interesting but unique solution.

---

# 41. Avoid Unnecessary UI

Before adding an element, ask:

> Does this help the user understand something or perform an action?

If not, it probably does not belong in the interface.

This applies especially to:

* decorative icons
* redundant labels
* unnecessary buttons
* excessive dividers
* duplicate information
* unnecessary dialogs

---

# 42. Qt Widget Guidelines

Use Qt Widgets according to their semantic purpose.

Examples:

| Purpose                    | Preferred Widget              |
| -------------------------- | ----------------------------- |
| Standard action            | `QPushButton`                 |
| Compact/icon navigation    | `QToolButton`                 |
| Text input                 | `QLineEdit`                   |
| Multi-line text            | `QTextEdit`                   |
| Selection                  | `QComboBox`                   |
| Boolean option             | `QCheckBox`                   |
| Mutually exclusive options | `QRadioButton`                |
| Date                       | `QDateEdit`                   |
| Numeric input              | `QSpinBox` / `QDoubleSpinBox` |
| Tabular data               | `QTableView`                  |
| List data                  | `QListView`                   |
| Multiple pages             | `QStackedWidget`              |
| Grouped visual container   | `QFrame`                      |
| Dialog                     | `QDialog`                     |

The table is a guideline, not an absolute rule.

A different widget may be appropriate when the interaction requires it.

---

# 43. Qt Layout Guidelines

Prefer layouts over manually positioned widgets.

Use:

```text
QVBoxLayout
```

for vertical structures.

Use:

```text
QHBoxLayout
```

for horizontal groups.

Use:

```text
QGridLayout
```

for structured two-dimensional layouts.

Use:

```text
QFormLayout
```

for label/input forms.

Use:

```text
QStackedWidget
```

for switching between pages.

Avoid excessive nesting when a simpler layout can achieve the same result.

---

# 44. Object Naming

Every important UI object should have a meaningful `objectName`.

Names should describe purpose rather than appearance.

Prefer:

```text
exerciseNameInput
```

over:

```text
lineEdit3
```

Prefer:

```text
addExerciseButton
```

over:

```text
pushButton2
```

Use consistent naming conventions throughout the application.

---

# 45. UI and Business Logic

UI code should not contain unnecessary business logic.

The interface should communicate with the existing application architecture rather than duplicating repository or database logic.

For example:

```text
UI
 ↓
Application logic
 ↓
Repository
 ↓
Database
```

The UI should primarily handle:

* displaying data
* collecting input
* user interaction
* visual state

---

# 46. Reusable Components

When the same UI pattern appears repeatedly, consider creating a reusable component.

Good candidates include:

* cards
* tags
* navigation buttons
* empty states
* form fields
* dialogs
* section headers

Do not create a reusable component solely because two widgets happen to look similar.

There should be a meaningful shared purpose.

---

# 47. New Feature Checklist

Before adding a new UI feature, check:

```text
□ What problem does this solve?
□ Which page should contain it?
□ Does an existing component already solve this?
□ Is the action clear?
□ Is the information hierarchy clear?
□ Does it follow the spacing system?
□ Does it follow the typography system?
□ Does it use existing colors?
□ Does it have appropriate interaction states?
□ Does it work at different window sizes?
□ Does it remain consistent with existing screens?
```

---

# 48. Final Guideline

Skillbase should feel predictable without feeling rigid.

Users should not have to learn a different interface for every hobby or feature.

The same principles should apply everywhere:

```text
Same navigation logic
Same visual hierarchy
Same component behavior
Same spacing system
Same interaction patterns
Same terminology
```

The content may change between hobbies, but the interface language should remain consistent.
