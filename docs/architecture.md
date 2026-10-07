# Software architecture

![Planned architecture](architecture.drawio.svg)

The diagram is the target architecture. Several boxes and its asynchronous
observation/action-status vocabulary are planned; they do not describe the
current blocking runner. Today, `MazeSolver` calls synchronous methods on
`IRobotIO` and receives each result before continuing. Later phases will add
stepwise coordination and bounded hardware operations without letting strategy
depend on Arduino or device drivers.

## Layers and dependency direction

- **Foundation:** shared `types`, the `grid` belief map, and `IRobotIO`.
- **Strategy:** current Pista A maze logic and future Pista B state machines.
- **Coordination (planned):** mode, elapsed-round deadline, scoring eligibility,
  recovery, and eventual state/action sequencing.
- **Subsystems and drivers (planned):** bounded physical actions and observations,
  with one thin `hal` wrapper per selected chip.
- **Safety (planned exception):** may gate motor output directly so an emergency
  stop does not travel up and down the stack.

Dependencies otherwise point downward. Python and image buffers stay on the Pi;
the Pi reports observations and never commands motors.

Current declared dependencies are explicit below. Native-only support depends on
production code, never the reverse.

| Module | Direct dependencies | Target |
|---|---|---|
| `types` | none | production and native |
| `grid` | `types` | production and native |
| `robot_io` | `types` | production and native |
| `maze` | `types`, `grid`, `robot_io` | production and native |
| `robot_io_fake` | `types`, `grid`, `robot_io` | native only |
| `maze_gen` | `types`, `grid` | native only |

## Current Pista A behavior

The local frame starts at `(0,0)`, facing North, with the boundary at the
robot's left. This is a preserved team convention, not a confirmed rulebook
guarantee. Coordinates use signed integers. The 5×5 `Maze` stores walls on
shared edges with `Unknown`, `Open`, and `Blocked` states. Unknown edges are
never passable.

`MazeSolver` owns its belief map and pose. `FakeRobotIO` owns an independent
truth map; keeping truth out of strategy prevents tests from granting knowledge
the robot has not sensed. The runner observes walls and tile color, asks a
strategy for a direction, turns, checks the tile ahead, and advances through
the blocking interface.

Exploration refuses a known red tile. The finishing phase can enter red to end
the normal run. A later phase may implement the rulebook's optional Bonus 1
return as an explicit post-red state; exploration must still never cross red.

The architecture does not yet implement real sensing, motor control, scoring,
terrain completion, display completion, persistence, or recovery. In particular,
`showColor()` returning is not evidence that a physical display was visible.

## Persistence and time

Belief restoration and Pista B section skipping are undecided and disabled.
Pre-mapping remains prohibited. If mid-round recovery is later allowed, a
trusted clock must preserve total elapsed round time across reset; otherwise
resume must be blocked. Saving a fresh six-minute remainder would be incorrect.

## Why this split

Pure logic can be tested before hardware arrives, while every physical claim
remains explicit. A real adapter can later implement `IRobotIO` without changing
the strategy contract. Test-only libraries are isolated so an ESP32 dependency
scan cannot silently include a simulated world.

For mentors: ask a student to point to the interface boundary, explain why the
belief and truth maps differ, and trace one dependency arrow. If an upper layer
includes a hardware header or a fake appears in the ESP32 build, the boundary
has been broken.

## Source conventions

Firmware stays C++17 with fixed-capacity storage, no heap, and no exceptions.
Coordinates use `std::int8_t` and fixed-width integers use their `std::` names;
`std::optional` is allowed. Public function bodies live in `.cpp` files rather
than inline in headers (inline `constexpr` configuration variables are fine).
Keep `.hpp` and `.cpp` together in the module and use namespace `maze` unless a
truly separate module needs its own namespace. `src/` contains only `main.cpp`.
