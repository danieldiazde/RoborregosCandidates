# RoBorregos Candidates 2026 — Slash Ultrathink

Software for the RoBorregos Candidates 2026 challenge. The tournament is
October 17, 2026. Development currently starts on a computer because no robot,
camera, display, or controller is connected and confirmed.

The PlatformIO configuration targets an ESP32-S3 development board, but that is
a software target rather than proof of the team's physical board. Start with the
[approved roadmap](docs/PLAN.md), [architecture](docs/architecture.md),
[module notes](docs/classes/README.md), [decisions](docs/adr/), and
[open questions](docs/OPEN_QUESTIONS.md).

## Repository map and status

| Area | Status and contents |
|---|---|
| `firmware/lib/types` | Built: directions, pose, actions/readings, capture, tile color, terrain |
| `firmware/lib/grid` | Built: 5×5 cells and edge-indexed, tri-state walls |
| `firmware/lib/robot_io` | Built: hardware-independent `IRobotIO` contract |
| `firmware/lib/maze` | Built: `MazeSolver`, `WallFollower`, `DfsExplorer`, `DfsStraightFirst`, `FrontierExplorer`, and BFS `PathPlanner` |
| `firmware/lib/robot_io_fake` | Built for native only: fake truth world, pose, clock, and counters |
| `firmware/lib/maze_gen` | Built for native only: seeded generator and independent validator |
| `firmware/src/main.cpp` | Inert Arduino `setup()`/`loop()`; links but operates no hardware |
| `firmware/lib/levels` | Placeholder only; Pista B is not implemented |
| `firmware/lib/robot`, `safety` | Planned coordination, recovery, and motor safety gate |
| `firmware/lib/drivetrain`, `odometry`, `perception`, `capture`, `indicator`, `vision_link` | Planned subsystems; no physical implementation |
| `firmware/lib/hal`, `persistence` | Planned device wrappers and bounded saved state |
| `vision/` | Placeholder Python files; no vision application or Pi deployment yet |
| `tools/` | Placeholder report/telemetry scripts; telemetry is development-only |
| `docs/` | Phase 0 handover notes; state/UART/pinout docs remain unfinished |

`firmware/include/config.h` is the one place for firmware tunables and unresolved
hardware configuration. Unknown pins and measurements stay explicitly unset.
Later hardware phases will add small bring-up programs before integrating drivers.

## Current maze software

`MazeSolver` maintains a belief map and asks an `ExplorationStrategy` for the
next direction. Strategy reaches sensors and actions only through the blocking,
pure C++ `IRobotIO` interface. Tests substitute `FakeRobotIO`, which holds a
separate ground-truth maze.

The current convention places the robot at local cell `(0,0)`, facing North,
with the boundary on its left. This awaits tournament confirmation. The map
stores each wall once on a cell edge and distinguishes unknown, open, and blocked
walls. Unknown is never passable. Coordinates are signed because searches inspect
neighboring positions.

During exploration the runner looks at the next tile and refuses to enter red.
It may enter red only in its finishing phase. The rulebook has an optional
Bonus 1 return after red; that exception is not implemented yet.

Known limitations include a fixed 60-second reserve rather than measured finish
costs, a 256-entry move log despite a 500-step loop limit, incomplete reset and
failure behavior, and uncalibrated simulated motion times. The generator creates
flat colored mazes; it is not a physical or terrain simulator.

## Build and test

From `firmware/`:

```bash
pio test -e native
pio run -e esp32
pio run -e bench
.pio/build/bench/program > ../tools/results.csv
```

The first two commands are required gates for every commit: both must pass with
zero compiler warnings. The benchmark CSV is generated output and is ignored.
The ESP32 build proves compilation and linking only, not boot or wiring safety.

Phase 0 validation: **39/39 native tests pass**, the ESP32 build succeeds, and
both report zero compiler warnings. The benchmark's 10,000 rows exactly match
the baseline behavior. The target build compiles production libraries and excludes
native test support. No hardware has been flashed or validated; see the
[delivery record](docs/PLAN.md#phase-0-delivery-record) for the checked boundaries.

## Engineering rules

- Dependencies point downward. Only safety may directly gate motor output.
- Strategy is pure C++17 and reaches the world through `IRobotIO`.
- Firmware uses fixed-capacity storage, no heap, and no exceptions. Public
  function bodies belong in `.cpp` files; `std::optional` is allowed.
- Use `std::int8_t` for coordinates and fixed-width `std::` integer types. Keep
  code in namespace `maze` unless a separate module clearly needs its own.
- `firmware/src/` contains only `main.cpp`; other firmware lives in `lib/`, with
  each module's `.hpp` and `.cpp` files together.
- Test fakes, generators, benchmarks, results, and build products stay off ESP32.
- Comments explain reasons, invariants, safety boundaries, and surprising rules.

Each phase updates affected docs and ADRs with the code. Generated files stay
untracked.
