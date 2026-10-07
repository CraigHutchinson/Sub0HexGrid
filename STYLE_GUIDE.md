style-profile: sub0

# Sub0HexGrid style guide

This library follows the Sub0 family C++ style profile. The authoritative rules,
exceptions and audit checks are maintained in the installed `sub0` profile.

Project-specific requirements:

- C++23, with public headers under `include/sub0hexgrid/`.
- Public functions and methods use `camelCase`; protocol names such as `begin`,
  `end` and operators keep their required spelling.
- Public interfaces use Doxygen comments with `@param` and `@return` tags as
  applicable. The first sentence is the brief; do not write `@brief`.
- Include this library's headers with quotes and a `sub0hexgrid/`-rooted path.
- Use `#pragma once` and `[[nodiscard]]` where ignoring a result is likely an
  error or wastes a computation.

Family-wide open decisions remain open; this file does not settle them.
