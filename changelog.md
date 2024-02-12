## 1.3.0

### Fixes
  - fix content block vertical size causing unnecessary scroll bar

### Improvements
  - optimize retrieving table row rect when rendering checked rows
  - fix inelegant workarounds forcing updates/recalculations/repaints
  - reimplement scroll area and scroll bar for content and menu by using CSS for styling and extending `QScrollArea` and `QScrollbar` for calculation adjustments to support all scrolling features (by mouse wheel and touchpad)
  - add `EventCallback` internal tool for future
