# Skillbase – UI Guidelines

Reference document for the visual and structural design language of Skillbase.
Applies to every UI task (new features, adjustments, redesigns).

## Design Philosophy

Modern, minimalist, calm, high-quality, desktop-oriented, information-oriented.

**Primary design reference: Linear** — not copied, but its principles adapted
(visual hierarchy, spacing, information density, interaction states).
Skillbase should feel like a standalone product, not like a Linear clone
and not like a "typical Qt application".

Guiding question before adding any new element:
> Does this element help the user understand something or perform an action?
> If not, it probably does not belong.

## Desktop-First

No mobile navigation, no touch controls, no web-style card layouts,
no unnecessary responsive breakpoints. Instead: high information density,
sensible use of horizontal space, compact controls.

## Color Roles

Dark, neutral palette. Colors are used semantically, not decoratively.

```
Background          – main background
Surface             – cards, panels, input areas, navigation
Surface Elevated    – emphasized surfaces
Border              – very subtle, for structure
Border Strong       – stronger separation, used sparingly
Text Primary / Secondary / Muted / Disabled
Accent              – #ff6f61 (sparingly: active navigation, primary actions,
                       focus states, important highlights)
Success / Warning / Error / Info
```

Not: full surfaces in orange, every button or every card in orange.
Never communicate errors through color alone (always add text or an icon).

*(Add concrete hex values from theme.qss here once they are final)*

## Spacing System

```
4px   minimum gaps
8px   tightly related elements
12px  compact components
16px  default padding
24px  section spacing
32px  large layout areas
```
No arbitrary values (7px, 13px, 19px) without a concrete reason.

## Typography

Hierarchy through size, weight, color, and spacing — not through many font sizes:
```
Page Title → Section Title → Card Title → Primary Value → Body → Secondary → Caption/Metadata
```
Important values (progress, goals, statistics) may be emphasized more strongly.

## Radius & Shadows

```
4px  small controls
6px  buttons
8px  cards, dialogs
```
Pill shapes only for tags, badges, or status indicators. Shadows used very sparingly —
no strong or large shadows. Hierarchy comes primarily from surface colors,
borders, spacing, and typography.

## Icons

SVG, functional, consistent size, stroke width, and alignment. No emoji as UI icons,
no purely decorative icons.

## Navigation

Two levels: `Application` → `Hobby` → `Hobby Section`. The active item is clearly
recognizable; accent color is used only as a subtle indicator, never as large
colored surfaces.

## Components

**Buttons:** Primary (important action, max. 1 per context) / Secondary / Ghost / Destructive.
Not every action should look like a primary action.

**States** (all interactive elements): Default, Hover, Pressed, Focused, Selected, Disabled.

**Cards:** subtle surface, thin border, small radius, consistent padding.
Do not automatically turn every area into a card.

**Metric Cards:** Label → Value (visually the most important part) → Context.

**Forms:** Label → Input → optional helper text. Errors directly at the field,
explanatory ("Goal must be greater than the current value." instead of "Invalid input.").

**Tags/Badges:** visually secondary, used for category, status, or level.

**Empty States:** explain what is missing + what the user can do + offer an action.

**Dialogs:** consistent width, padding, title, and button order across all dialogs.
Confirmation dialogs only for actions that are hard to reverse.

**Reusable component names** (if implemented as dedicated widgets):
`SidebarNavigationItem`, `PrimaryButton`, `SecondaryButton`, `Card`, `MetricCard`,
`SectionHeader`, `Tag`, `StatusBadge`, `EmptyState`, `InputField`, `Dialog`

## Qt Widget Mapping

| Purpose             | Widget           |
|---------------------|------------------|
| Standard action     | `QPushButton`    |
| Compact navigation  | `QToolButton`    |
| Text input          | `QLineEdit`      |
| Multi-line text     | `QTextEdit`      |
| Selection           | `QComboBox`      |
| Boolean             | `QCheckBox`      |
| Date                | `QDateEdit`      |
| Integer             | `QSpinBox`       |
| Decimal             | `QDoubleSpinBox` |
| Table               | `QTableView`     |
| List                | `QListView`      |
| Pages               | `QStackedWidget` |
| Container           | `QFrame`         |
| Dialog              | `QDialog`        |

Layouts: `QVBoxLayout` / `QHBoxLayout` / `QGridLayout` / `QFormLayout` / `QStackedLayout`.
No `setGeometry()` or absolute positioning without a concrete reason.

## Object Names

Semantic, not generic: `addExerciseButton`, `exerciseNameInput`, `exerciseCard`
instead of `pushButton2`, `lineEdit3`. **Do not rename existing object names without reason** —
before any change, verify all references (`ui->`, `findChild`, `connect()`, QSS selectors).

## `.ui` vs. `.qss` vs. `.cpp`

- **.ui** → widget hierarchy, layouts, static structure, size policies, margins/spacing
- **.qss** → colors, typography, borders, radius, hover/selected/disabled states
- **.cpp** → dynamic widgets, runtime-dependent states, custom widget logic

## Accessibility & Keyboard

Never communicate errors through color alone. Sufficient click areas, visible focus state,
logical tab order, sensible Enter/Escape behavior.

## Explicitly Avoid

Unnecessary dialogs, cards, or borders; heavy shadows or gradients; oversized buttons
or headlines; random spacings; inconsistent components; decorative icons; unnecessary
animations; mobile or web UI patterns.

## Guiding Principle

> For every visual decision: Linear as the primary guideline, adapted to Skillbase.
> For every functional decision: existing Skillbase functionality takes priority.
> For every component decision: consistency and reuse before one-off solutions.