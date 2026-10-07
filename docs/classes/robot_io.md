# Robot IO

Status: **interface and native fake implemented; real adapter planned**.

`IRobotIO` is the pure C++ boundary used by strategy. Its current calls are
blocking: sensor calls return a reading, action calls return an `ActionResult`,
and `millisRemaining()` returns the fake/current clock value. It has no Arduino
types. `FakeRobotIO` lives in the native-only `robot_io_fake` library and combines
a separate truth maze, simulated pose, counters, and uncalibrated motion costs.

Example: `MazeSolver` calls `readTileAhead()`. The fake looks into its truth map;
a later real adapter will ask a selected sensor. The solver sees only a
`TileColor`, so the same strategy remains usable in both cases.

## Invariants and failure behavior

- Production code depends on `IRobotIO`, never on `FakeRobotIO`.
- Fake truth never enters the solver's belief except through observations.
- Test support and generators never compile into the ESP32 image.
- `showColor()` currently records a fake value; it does not prove visibility.
- Fake line and grip behavior are placeholders and fake costs are not measured.

Native robot-IO tests cover motion, blocking, and timing; strategy tests exercise
wall and tile observations. Line sensing and grip simulation remain placeholders,
not validated Pista B behavior. Hardware acceptance waits for selected devices
and later bring-up programs.

## How to explain this to a mentor

`IRobotIO` is a seam: the maze algorithm asks for capabilities without knowing
which chip provides them. The fake provides repeatable laptop evidence, while
isolation prevents simulated truth from leaking into production firmware.
