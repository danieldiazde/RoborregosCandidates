# Motion vocabulary

Status: **shared types implemented; drivetrain planned**.

`types/Motion.hpp` defines hardware-independent requests and observations:
relative turns, strafing direction, `ActionResult`, `WallReading`, and
`LineReading`. These types let strategy compile natively. They do not move a
robot, guarantee a maneuver completed, or identify any sensor.

An action reports `Ok`, `Blocked`, `Aborted`, or `Failed`. Later hardware code
must make those outcomes bounded and meaningful; Phase 0 has only fake behavior.

Example: a strategy can request a right turn through `IRobotIO`. The fake updates
its heading immediately and returns `Ok`; a future drivetrain adapter must return
only after its own completion/failure contract is satisfied.

## Invariants and tests

- The vocabulary has no Arduino dependency.
- Safety abortion is distinct from a blocked path and hardware failure.
- Physical motion, timing, line sensing, and warning limits are unverified.

Native robot-IO and maze tests exercise fake outcomes. They are not motor tests.

## How to explain this to a mentor

These enums are a common language between decision code and an adapter. They say
what was requested and how it ended, without naming a motor driver or sensor.
