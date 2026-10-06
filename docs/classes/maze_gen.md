# Maze generator and validator

Status: **implemented for native tests and benchmarks only**.

`MazeGenerator` builds deterministic 5×5 fixtures from a seed, including loops,
a corner start, and a red corner. `MazeValidator` independently checks structural
properties. The module is test support and never belongs in the ESP32 image.

Example: a benchmark supplies a seed, generates a truth maze, validates it, and
passes that truth to `FakeRobotIO`. Strategy learns a separate belief only through
the fake's observations.

## Current limitations

- Generated cells are flat and colored; there is no terrain simulation.
- The validator's connectivity check may pass through red, so it does not prove
  that every required cell is reachable before exploration enters red.
- Its inward-heading check does not independently prove that the boundary is on
  the robot's left. A separate test checks that convention.
- `extraOpenings` is narrowed to a signed 8-bit value before clamping. Large
  inputs can therefore behave incorrectly; the bounds fix belongs to Phase 1.

These limits mean a valid generated maze is a useful algorithm fixture, not proof
that a tournament maze, terrain, or full scoring path is supported.

## How to explain this to a mentor

The generator creates repeatable questions for an algorithm; the validator checks
that the questions are structurally sensible. Neither one simulates the physical
course, and validator coverage must not be mistaken for strategy coverage.
