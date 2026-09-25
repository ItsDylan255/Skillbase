# Skillbase UI Components

This document defines the reusable UI components used throughout Skillbase.

Components should provide a consistent visual and interaction pattern for recurring interface elements.

New UI should reuse existing components whenever possible instead of creating new variations.

---

# 1. Component Principles

Skillbase components should be:

* reusable
* predictable
* consistent
* accessible
* visually minimal
* easy to maintain

A component should have one clear purpose.

Avoid creating components that combine unrelated functionality.

---

# 2. Component Categories

Skillbase components are grouped into:

```text
Navigation
Actions
Content
Forms
Feedback
Data
Layout
```

---

# 3. Navigation Components

## 3.1 Sidebar

The sidebar provides global application navigation.

### Structure

```text id="d07j6c"
Sidebar
├── App Header
├── Navigation
└── Optional Secondary Area
```

### Responsibilities

* application navigation
* active page indication
* access to global areas

### Guidelines

The sidebar should remain visually stable while navigating through the application.

It should not contain hobby-specific navigation.

---

## 3.2 Sidebar Navigation Item

A sidebar navigation item represents one global destination.

Example:

```text id="l1k2ac"
◉ Overview
```

### States

```text id="5w48gn"
Default
Hover
Selected
Focused
Disabled
```

### Selected State

The selected item should be clearly distinguishable through:

* subtle surface change
* accent indicator
* text/icon emphasis

Avoid making the selected item significantly larger than other navigation items.

---

## 3.3 Hobby Navigation

Hobby navigation provides access to the sections of the currently selected hobby.

Example:

```text id="6b6b15"
Dashboard
Routines
Exercises
History
Progress
Goals
Timeline
Roadmap
```

The active section should use the same interaction principles as global navigation.

---

# 4. Action Components

## 4.1 Primary Button

The primary button represents the most important action in a context.

Examples:

```text id="2w5n1v"
Add Exercise
Save
Start Routine
Create Goal
```

### Characteristics

* strong visual emphasis
* Skillbase accent
* clear label
* appropriate icon where useful

Only use primary emphasis for actions that deserve it.

---

## 4.2 Secondary Button

Secondary buttons represent supporting actions.

Examples:

```text id="z1xy90"
Cancel
Edit
Details
```

They should remain visually quieter than primary buttons.

---

## 4.3 Ghost Button

Ghost buttons are lightweight actions.

Typical use cases:

* toolbar actions
* secondary actions
* contextual actions
* navigation-like controls

They should not visually compete with primary actions.

---

## 4.4 Destructive Button

Destructive buttons are used for actions such as:

```text id="d9t9de"
Delete
Remove
Permanently Delete
```

They use the semantic error color where appropriate.

Destructive actions should not accidentally look like ordinary primary actions.

---

# 5. Icon Button

An icon button performs a compact action using an icon.

Examples:

```text id="1hz0e0"
Edit
Delete
More
Settings
```

Icon buttons should have:

* a clear tooltip when the icon alone is ambiguous
* visible hover state
* visible focus state
* sufficient clickable area

Avoid using unfamiliar icons without supporting text or a tooltip.

---

# 6. Card

Cards group related information.

### Structure

```text id="20t9r9"
Card
├── Header
│   ├── Title
│   └── Optional Action
└── Content
```

### Typical uses

* dashboard metrics
* progress summaries
* recent activity
* goals
* notes
* overview information

### Visual properties

Cards should use:

* surface background
* subtle border
* consistent padding
* restrained radius

Cards should not automatically contain buttons or actions unless they serve a clear purpose.

---

# 7. Metric Card

A metric card displays one important value.

Example:

```text id="x7h5xy"
Progress

68 %

+4 % this month
```

### Structure

```text id="85h6i8"
Metric Card
├── Label
├── Value
└── Optional Context
```

The value should be the most visually prominent element.

---

# 8. Activity Card

An activity card displays recent activity.

Example:

```text id="ypx6rc"
Recent Activity

Pull-ups
Completed 3 sets

Yesterday
```

Activity cards should prioritize:

1. activity
2. relevant context
3. time

---

# 9. Section

A section groups related content.

### Structure

```text id="r4r1vp"
Section
├── Header
│   ├── Title
│   └── Optional Action
└── Content
```

Sections should be used instead of arbitrary collections of widgets.

---

# 10. Section Header

A section header establishes the context of a content group.

Example:

```text id="s5j4h8"
Recent Activity                         [ View All ]
```

The title should be visually stronger than supporting content.

Actions should remain secondary unless they represent the primary action of the entire page.

---

# 11. Tag

A tag represents metadata.

Examples:

```text id="x3a4m6"
Technique
Strength
Beginner
Active
```

Tags should be compact and visually secondary.

---

# 12. Status Badge

A status badge communicates a state.

Examples:

```text id="z6j5f2"
Active
Archived
Completed
In Progress
```

Status badges may use semantic colors when this improves recognition.

Color should not be the only indication of state.

---

# 13. Empty State

An empty state is displayed when a section contains no data.

### Structure

```text id="kn6xpj"
Empty State
├── Optional Icon
├── Title
├── Description
└── Optional Action
```

Example:

```text id="a3m2s7"
No exercises yet

Create your first exercise to start tracking your progress.

[ Add Exercise ]
```

The action should directly address the empty state where appropriate.

---

# 14. Input Field

A standard single-line input.

### Structure

```text id="1s5n8p"
Label
Input
Optional Supporting Text
```

### States

```text id="m3n6n9"
Default
Hover
Focused
Error
Disabled
```

The focused state should be clearly visible.

---

# 15. Text Area

A multi-line text input used for longer content.

Examples:

* descriptions
* notes
* comments

Text areas should provide enough vertical space for their intended content.

---

# 16. Select

A select control allows the user to choose from predefined options.

Examples:

```text id="3h5s4j"
Category
[ Technique ▼ ]
```

Use a select when the available options are known and relatively limited.

Do not use a select when users need to enter arbitrary text.

---

# 17. Checkbox

Checkboxes represent independent boolean options.

Examples:

```text id="k2y6t3"
☐ Show archived exercises
☐ Enable notifications
```

The label should clearly describe what enabling the option does.

---

# 18. Radio Group

Radio buttons are appropriate when exactly one option should be selected from a small group.

Example:

```text id="j4q7x2"
Tracking Mode

(•) Repetitions
( ) Weight
( ) Duration
```

Use a select instead when the list becomes large.

---

# 19. Numeric Input

Use numeric controls when a value has a defined numeric meaning.

Examples:

* repetitions
* weight
* duration
* target value

Qt widgets such as:

```cpp id="z4r5s1"
QSpinBox
QDoubleSpinBox
```

should be preferred where appropriate.

Units should be displayed clearly.

---

# 20. Date Input

Use date controls for dates that users need to enter or change.

Examples:

* goal deadlines
* activity dates
* scheduled routines

Qt's:

```cpp id="2a4n6x"
QDateEdit
```

may be used where appropriate.

---

# 21. Dialog

Dialogs are focused temporary workflows.

### Structure

```text id="8d3j4m"
Dialog
├── Header
│   └── Title
├── Content
└── Actions
```

Dialogs should remain focused.

Avoid turning dialogs into miniature applications.

---

# 22. Confirmation Dialog

A confirmation dialog asks the user to confirm an important action.

Example:

```text id="j8w3m1"
Delete Exercise?

This will permanently delete "Pull-ups".

[ Cancel ] [ Delete ]
```

Use primarily for destructive or difficult-to-reverse actions.

---

# 23. Toast / Temporary Feedback

Temporary feedback may be used for actions that do not require user interaction.

Examples:

```text id="5c9n7w"
Exercise saved
```

```text id="k5s8m2"
Routine completed
```

Temporary feedback should be:

* short
* unobtrusive
* easy to understand

Do not use it for important errors that require user action.

---

# 24. Inline Error

An inline error appears close to the affected field or content.

Example:

```text id="q8x2v6"
Goal Value

[ -10 ]

Goal must be greater than zero.
```

Inline errors should explain how the user can correct the problem.

---

# 25. Progress Bar

Progress bars represent measurable progress toward a target.

Example:

```text id="x5n9z2"
Progress

68 %

██████████████░░░░░░
```

Use progress bars only when the percentage or range has a meaningful interpretation.

---

# 26. Progress Summary

A progress summary combines a current value with a target.

Example:

```text id="c2x7m4"
Pull-ups

Current
8 reps

Goal
15 reps

██████████░░░░░░░
```

The current value and target should be easy to compare.

---

# 27. List Item

A list item represents one object in a collection.

Example:

```text id="r8s4p1"
Pull-ups
Strength · Upper Body
Last performed yesterday
```

A list item may contain:

```text id="v2h5k7"
Title
Metadata
Status
Action
```

Only include information that helps identify or act on the item.

---

# 28. Table Row

A table row represents one structured record.

Rows should support:

* clear alignment
* hover feedback
* selection where appropriate
* contextual actions

Avoid placing too many actions directly into every row.

Additional actions may be placed inside an overflow menu.

---

# 29. Overflow Menu

An overflow menu contains secondary actions that would otherwise clutter the interface.

Example:

```text id="m7v2c8"
[ ... ]

Edit
Archive
Delete
```

Use it for actions that are important but not frequently needed.

Do not hide the primary action inside an overflow menu.

---

# 30. Tabs

Tabs may be used when several closely related views need to be switched without leaving the current context.

Tabs should not replace the primary application navigation.

Use tabs for closely related content, not unrelated sections of the application.

---

# 31. Toolbar

A toolbar groups frequently used contextual actions.

Example:

```text id="g5n8q2"
[ Search ] [ Filter ] [ Sort ]                  [ Add ]
```

Toolbars should remain compact.

Actions should be grouped logically.

---

# 32. Search Field

A search field should clearly communicate what it searches.

Example:

```text id="h4r7m3"
⌕ Search exercises...
```

A clear/reset action should be available when a query is entered.

---

# 33. Filter Control

Filters allow users to reduce a dataset.

Example:

```text id="p5s2n8"
[ Category ] [ Status ] [ Archived ]
```

Active filters should be visually identifiable.

Users should be able to remove filters easily.

---

# 34. Sort Control

Sort controls allow users to change the order of data.

Example:

```text id="n7k3r5"
Sort: Last performed ▼
```

The current sorting mode should always be clear.

---

# 35. Navigation Header

A navigation header identifies the current context.

Example:

```text id="b4m8x2"
Guitar

Exercises
```

It may contain:

* context
* page title
* actions

The header should remain visually lightweight.

---

# 36. Hobby Header

The hobby header identifies the currently selected hobby.

Example:

```text id="c7p3m9"
Guitar
Electric Guitar · Skill Development
```

It may contain hobby-specific actions.

The header should establish context before the user reaches the content.

---

# 37. Page Header

The page header is the primary introduction to a page.

Example:

```text id="q6v4s1"
Exercises

Track and manage your exercises.

                                      [ Add Exercise ]
```

It should normally contain:

* title
* optional description
* primary action

---

# 38. Detail View

A detail view displays the complete information for one entity.

Examples:

* exercise details
* routine details
* goal details
* hobby details

A detail view should prioritize the entity's identity and important information before secondary metadata.

---

# 39. Editor

An editor allows users to create or modify an entity.

Example:

```text id="w5r2n7"
Edit Exercise

Name
[ Pull-ups ]

Category
[ Strength ]

Goal
[ 15 ]

Description
[ ... ]

                    [ Cancel ] [ Save ]
```

Editors should clearly distinguish between:

* editable fields
* read-only information
* actions

---

# 40. Collection View

A collection view displays multiple objects.

Examples:

* exercise collection
* hobby collection
* routines
* goals

A collection view typically consists of:

```text id="e3n7p5"
Page Header
├── Search / Filters
└── Collection
```

The collection itself may use cards, lists, or tables depending on the data.

---

# 41. Dashboard Grid

Dashboard grids arrange summary components.

Example:

```text id="x2m6r8"
┌──────────────┐ ┌──────────────┐
│ Today        │ │ Goals        │
└──────────────┘ └──────────────┘

┌───────────────────────────────┐
│ Recent Activity               │
└───────────────────────────────┘
```

The grid should adapt to available horizontal space.

Do not create a fixed layout that only works at one window size.

---

# 42. Component States

Reusable components should define their states consistently.

At minimum, interactive components should consider:

```text id="y7p4m2"
Default
Hover
Pressed
Focused
Selected
Disabled
```

Components that support validation should additionally consider:

```text id="a6n8q3"
Error
```

Components representing asynchronous operations may additionally use:

```text id="f2r5k9"
Loading
```

---

# 43. Component Naming

Component names should describe their purpose.

Prefer:

```text id="z4x7c1"
ExerciseCard
MetricCard
SidebarNavigationItem
EmptyState
SectionHeader
PrimaryButton
```

Avoid names based only on appearance:

```text id="m8q2v6"
DarkBox
OrangeButton
RoundedPanel
BigCard
```

The visual appearance may change; the semantic purpose should remain stable.

---

# 44. Qt Implementation

Components should use Qt Widgets.

Depending on complexity, reusable components may be implemented as:

* existing Qt widgets with shared styling
* custom `QWidget` subclasses
* `.ui` forms
* reusable layout structures
* shared QSS rules

Do not create a custom widget when a standard Qt widget already provides the required behavior.

---

# 45. Component Reuse

Before creating a new component, ask:

```text id="r3m7v5"
Does an existing component already solve this?
```

If yes, reuse it.

If the component is almost identical but has a small variation, prefer a configurable existing component rather than duplicating it.

---

# 46. Component Documentation

New reusable components should document:

* purpose
* intended usage
* supported states
* important properties
* interaction behavior

Example:

```text id="h6n2q8"
Component: MetricCard

Purpose:
Display one important numerical metric.

Contains:
- label
- value
- optional context

Use when:
The value represents an important summary metric.

Do not use when:
The content is a complex dataset.
```

---

# 47. Component Design Rule

Components should be visually recognizable by their purpose.

For example:

```text id="p8m4r6"
Primary Button
→ Important action

Tag
→ Metadata

Status Badge
→ State

Card
→ Grouped information

List Item
→ Individual collection item

Table
→ Structured comparison
```

The same visual component should not represent several unrelated concepts.

---

# 48. Final Component Rule

When building a new Skillbase screen:

1. Identify the content.
2. Identify the required interactions.
3. Select existing components.
4. Arrange them using the UI Guidelines.
5. Apply the Design System.
6. Only create a new component if an existing one cannot represent the requirement appropriately.

The goal is not to maximize the number of components.

The goal is to create a small, coherent set of components that can build the entire Skillbase interface.

---

# 49. Skillbase Component Library

The initial component library consists of:

```text
Navigation
├── Sidebar
├── Sidebar Navigation Item
└── Hobby Navigation

Actions
├── Primary Button
├── Secondary Button
├── Ghost Button
├── Destructive Button
└── Icon Button

Content
├── Card
├── Metric Card
├── Activity Card
├── Section
├── Section Header
├── List Item
├── Detail View
└── Collection View

Metadata
├── Tag
└── Status Badge

Forms
├── Input Field
├── Text Area
├── Select
├── Checkbox
├── Radio Group
├── Numeric Input
└── Date Input

Feedback
├── Empty State
├── Inline Error
├── Toast
├── Progress Bar
└── Progress Summary

Data
├── Table
├── Table Row
├── Search
├── Filter
└── Sort

Layout
├── Page Header
├── Navigation Header
├── Hobby Header
├── Dashboard Grid
└── Toolbar

Dialogs
├── Dialog
└── Confirmation Dialog
```

This library should evolve only when new requirements justify additional reusable patterns.
