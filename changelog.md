## 1.3.0

### New features
  - implement star buttons for table rows

### Fixes
  - fix content block vertical size causing unnecessary scroll bar
  - make menu more adaptive by replacing menu items implementation
  - disable add/edit buttons when they should not be enabled to fix unexpected behaviour

### Improvements
  - optimize retrieving table row rect when rendering checked rows
  - fix inelegant workarounds forcing updates/recalculations/repaints
  - reimplement scroll area and scroll bar for content and menu by using CSS for styling and extending `QScrollArea` and `QScrollbar` for calculation adjustments to support all scrolling features (by mouse wheel and touchpad)
  - make table cells size adaptive based on the contents
  - add `EventCallback` internal tool for future
  - migrate from XML files to SQLite database
