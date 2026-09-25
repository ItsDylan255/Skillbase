# Skillbase Design System

This document defines the visual foundation of Skillbase.

The goal is to provide a consistent, modern, minimal, and desktop-focused interface across the entire application.

Skillbase takes visual inspiration from modern productivity applications such as Linear while maintaining its own visual identity.

---

## 1. Design Principles

Skillbase follows these core visual principles:

### Minimal

The interface should contain only elements that provide value.

Avoid:

* unnecessary decoration
* excessive borders
* large shadows
* gradients
* oversized controls
* excessive use of colors

### Clear

The visual hierarchy should make it immediately obvious:

* where the user is
* what the current page represents
* which actions are available
* which information is important
* which elements are interactive

### Consistent

The same visual rules should be used throughout the application.

Components that serve the same purpose should look and behave consistently.

### Dense, but not crowded

Skillbase is a desktop application and should use the available screen space efficiently.

Information should be compact enough to remain useful while maintaining sufficient whitespace for readability.

### Functional

Visual design should support the purpose of the application.

Decoration should never take priority over usability.

---

# 2. Visual Direction

The overall visual direction is:

* modern
* minimalist
* dark
* desktop-oriented
* subtle
* structured
* information-focused

The interface uses neutral surfaces with a single primary accent color.

The design should feel polished without looking overly decorative.

---

# 3. Color System

Skillbase uses a dark neutral base with an orange accent.

Colors should be defined centrally rather than individually throughout the application.

## 3.1 Base Colors

| Token              | Purpose                                                 |
| ------------------ | ------------------------------------------------------- |
| `background`       | Main application background                             |
| `surface`          | Standard elevated UI surface                            |
| `surface-elevated` | Higher-level surface such as dialogs or important cards |
| `border`           | Subtle separation between elements                      |
| `border-strong`    | Stronger separation where required                      |

The neutral surfaces should be visually close to each other.

Large differences in brightness should be avoided.

---

## 3.2 Text Colors

| Token            | Purpose                             |
| ---------------- | ----------------------------------- |
| `text-primary`   | Main text and important information |
| `text-secondary` | Supporting information              |
| `text-muted`     | Low-priority information            |
| `text-disabled`  | Disabled controls                   |

Text hierarchy should primarily be created through:

1. font size
2. font weight
3. contrast

rather than through many different colors.

---

## 3.3 Accent Color

The primary Skillbase accent color is:

```text
#ff6f61
```

The accent is used for:

* active navigation
* primary actions
* important interactive elements
* focus indicators
* selected states
* progress indicators where appropriate

The accent should not be used everywhere.

It should retain visual meaning by remaining relatively uncommon in the interface.

---

## 3.4 Semantic Colors

Semantic colors may be used when they communicate a specific state.

| Token     | Purpose                             |
| --------- | ----------------------------------- |
| `success` | Successful action or positive state |
| `warning` | Warning or attention required       |
| `error`   | Error or destructive state          |
| `info`    | Informational state                 |

Semantic colors should be used sparingly.

They should communicate meaning rather than serve as decoration.

---

# 4. Typography

Skillbase uses a simple typographic hierarchy.

## 4.1 Hierarchy

The interface should distinguish between:

```text
Page Title
Section Title
Card Title
Body
Secondary
Caption
```

### Page Title

Used for the primary title of a page.

Characteristics:

* large
* strong
* high contrast

### Section Title

Used to separate major content sections.

Characteristics:

* medium-large
* semibold
* high contrast

### Card Title

Used for titles inside cards and smaller content areas.

Characteristics:

* medium
* semibold
* high contrast

### Body

Used for normal content and descriptions.

Characteristics:

* normal weight
* comfortable line height
* primary or secondary text color

### Secondary

Used for supporting information.

Characteristics:

* smaller or lower contrast
* secondary text color

### Caption

Used for metadata and low-priority information.

Characteristics:

* small
* muted
* never used for essential information

---

# 5. Spacing

Skillbase uses a small, consistent spacing scale.

Preferred values:

```text
4px
8px
12px
16px
24px
32px
```

These values should be reused throughout the interface.

Examples:

```text
4px  → very small internal spacing
8px  → related elements
12px → compact component spacing
16px → standard component padding
24px → section spacing
32px → major layout separation
```

Avoid introducing arbitrary spacing values unless a specific layout requirement justifies them.

---

# 6. Layout

The interface should use generous but controlled whitespace.

Content should generally be structured using:

* horizontal groups
* vertical groups
* grids
* cards
* sections

Qt layouts should be preferred over manually positioned widgets.

Preferred Qt layouts include:

```cpp
QVBoxLayout
QHBoxLayout
QGridLayout
QFormLayout
QStackedLayout
```

Absolute positioning should generally be avoided.

---

# 7. Borders

Borders should be subtle.

They primarily exist to:

* separate surfaces
* define component boundaries
* improve visual hierarchy

Borders should not dominate the interface.

Avoid heavy outlines around every element.

Cards, inputs, tables, and dialogs may use subtle borders where necessary.

---

# 8. Border Radius

Skillbase uses restrained corner rounding.

Preferred values:

```text
4px
6px
8px
```

The radius should depend on the component.

Examples:

```text
Small controls → 4px
Buttons → 6px
Cards → 8px
Dialogs → 8px
```

Avoid excessive rounding or pill-shaped components unless the component is specifically intended to be a tag, badge, or status indicator.

---

# 9. Elevation

Skillbase does not rely heavily on shadows.

Hierarchy should primarily be created through:

* surface color
* borders
* spacing
* typography

Shadows may be used very subtly for elements that visually float above the main interface, such as dialogs.

Large or dramatic shadows should be avoided.

---

# 10. Icons

Icons should be:

* simple
* consistent
* functional
* visually balanced

SVG icons are preferred.

Icons should have consistent sizing within the same component category.

Icons should communicate meaning rather than being used purely for decoration.

When an icon and text are used together, the icon should support the text rather than replace it unnecessarily.

---

# 11. Interactive States

Interactive components should provide clear visual feedback.

The main states are:

```text
Default
Hover
Pressed
Focused
Selected
Disabled
```

### Default

The normal appearance of the component.

### Hover

A subtle visual change indicating that the element is interactive.

### Pressed

A temporary visual change while the user activates the element.

### Focused

A clear but unobtrusive indication that the element currently has keyboard focus.

### Selected

Used for active navigation, selected items, or active options.

The Skillbase accent color may be used here.

### Disabled

Disabled elements should have reduced contrast while remaining understandable.

Disabled controls should not look like normal controls.

---

# 12. Buttons

Buttons are divided into several visual categories.

## Primary

Used for the most important action in a context.

Examples:

```text
Save
Create Exercise
Start Routine
Add Hobby
```

Primary buttons may use the Skillbase accent.

---

## Secondary

Used for normal supporting actions.

Examples:

```text
Edit
Details
Cancel
```

Secondary buttons should have less visual emphasis than primary actions.

---

## Ghost

Used when an action should remain visually lightweight.

Commonly used for:

* navigation
* toolbar actions
* secondary contextual actions

---

## Destructive

Used for actions that can remove or permanently change data.

Examples:

```text
Delete
Remove
```

Destructive actions should use the semantic error color where appropriate.

---

# 13. Cards

Cards group related information.

A card generally consists of:

```text
Card
├── Header
│   ├── Title
│   └── Optional Action
└── Content
```

Cards should use:

* subtle surface contrast
* subtle border
* consistent padding
* restrained corner radius

Cards should not contain unnecessary decoration.

Multiple cards should be able to appear next to each other where the available space allows it.

---

# 14. Tags and Badges

Tags and badges represent compact pieces of metadata.

Examples:

```text
Technique
Active
Beginner
Archived
Goal
```

They should be:

* compact
* easy to scan
* visually distinct
* less prominent than primary actions

Tags may use a slightly rounded shape.

Unlike normal buttons, tags are primarily informational unless explicitly interactive.

---

# 15. Forms

Forms should use a clear vertical hierarchy.

A typical field follows:

```text
Label
Input
Optional supporting text
```

Example:

```text
Name

[ Guitar Practice              ]

Category

[ Technique                    ]
```

Form fields should have consistent:

* height
* spacing
* border
* radius
* focus state

Labels should remain visually connected to their inputs.

---

# 16. Navigation

Navigation should remain compact and predictable.

The primary navigation consists of:

* application-level navigation
* hobby-level navigation

Navigation items should clearly communicate:

* icon
* label
* current selection

The active item should be visually distinguishable without becoming excessively bright or large.

The Skillbase accent may be used for the active state.

---

# 17. Sidebar

The sidebar is a persistent application-level navigation area.

Conceptually:

```text
Sidebar
├── Application Header
├── Main Navigation
│   ├── Overview
│   ├── Statistics
│   └── Settings
└── Optional Secondary Area
```

The sidebar should:

* remain visually distinct from the content area
* use compact spacing
* avoid unnecessary decoration
* clearly indicate the active page

---

# 18. Content Areas

The main content area should provide enough space for the actual information.

A typical page structure is:

```text
Page
├── Header
│   ├── Title
│   └── Actions
│
└── Content
    ├── Section
    ├── Section
    └── Section
```

Page headers should establish the visual hierarchy before the user reaches the detailed content.

---

# 19. Tables and Lists

Tables and lists should prioritize scanability.

Avoid excessive borders between every item.

Use:

* spacing
* subtle separators
* hover states
* typography
* alignment

to create structure.

Important values should be visually prioritized.

---

# 20. Dialogs

Dialogs should be visually separated from the main application.

A dialog generally follows:

```text
Dialog
├── Title
├── Content
└── Actions
```

Actions should be placed consistently.

The primary action should be visually distinguishable.

Destructive actions should not accidentally appear as the primary action.

---

# 21. Empty States

Empty states should explain what is happening and, where appropriate, what the user can do next.

Example:

```text
No exercises yet

Create your first exercise to start tracking your progress.

[ Add Exercise ]
```

Avoid empty pages that simply contain nothing.

---

# 22. Scrollbars

Scrollbars should remain visually subtle.

They should:

* fit the dark theme
* avoid unnecessary width
* remain discoverable
* provide clear interaction states

Scrollbars should not visually compete with the content.

---

# 23. Qt Implementation

The design system is implemented using Qt 6 Widgets.

Primary technologies:

```text
Qt Widgets
Qt Style Sheets (QSS)
Qt layouts
SVG resources
```

The design system should be implemented centrally wherever practical.

Avoid repeating identical styling definitions throughout individual widgets.

For reusable styling, prefer shared QSS rules and reusable components.

---

# 24. Design Rule

When introducing a new UI element, ask:

1. What is its purpose?
2. Which existing component does it resemble?
3. Can an existing component be reused?
4. Which spacing values apply?
5. Which text hierarchy applies?
6. Which interaction states does it need?
7. Does it introduce a new visual pattern unnecessarily?

A new component should only introduce a new visual pattern when an existing pattern cannot represent the required interaction or information effectively.

---

# 25. Overall Goal

The Skillbase interface should feel like one coherent application rather than a collection of individually designed screens.

Every part of the interface should follow the same visual language:

```text
Minimal
Clear
Consistent
Structured
Functional
Modern
Desktop-focused
```

The design system should evolve as Skillbase evolves, but new UI should always build upon these existing principles rather than creating unrelated visual patterns.
