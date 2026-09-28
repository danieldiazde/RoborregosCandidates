# RoBorregos Candidates 2026 - Slash Ultrathink

- Controller: ESP32-S3, C++17 / Arduino framework / FreeRTOS
- Vision: Raspberry Pi Zero 2 W, Python + OpenCV, linked over UART

See [`docs/architecture.md`](docs/architecture.md) for the layered design.

---

## Repo layout

```
docs/         design documents
firmware/     everything running on the ESP32, plus its tests and benchmark
vision/       Python running on the Raspberry Pi
tools/        laptop-side helpers (analysis, telemetry)
```

### `docs/`

| Path | Contents |
|---|---|
| `architecture.md` | the layered design and the rules behind it |
| `architecture.drawio.svg` | the diagram |
| `states.md` | robot state machine, including checkpoint restart |
| `uart.md` | ESP32 ↔ Pi message table and failure handling |
| `pinout.md` | GPIO map |
| `classes/` | one class diagram per firmware module |
| `adr/` | architecture decision records |

### `firmware/`

PlatformIO project. Open `candidates.code-workspace` from the repo root to get
firmware, vision and docs in one VS Code window.

```
platformio.ini      build configuration: esp32, native (tests), bench
src/main.cpp        entry point — creates FreeRTOS tasks, nothing else
lib/<module>/       one folder per module, .hpp and .cpp together
test/test_<name>/   native unit tests, one suite per folder
bench/              strategy benchmark, runs on the laptop
```

Status: **built** = implemented and tested; **planned** = folder exists, no code yet.

**Foundation** — pure C++, no dependencies on hardware

| Module | Status | Contents |
|---|---|---|
| `types/` | built | `Direction` (turn arithmetic, `deltaX/Y`, `directionTo`), `Pose`, `Motion` (`RelativeTurn`, `Strafe`, `ActionResult`, `WallReading`, `LineReading`), `Capture` (`GripCommand`, `Possession`), `TileColor`, `Terrain` |
| `grid/` | built | `Maze` (5×5, edge-indexed walls, tri-state `WallState`), `Cell` (visited, color, terrain) |
| `robot_io/` | built | `IRobotIO` — the only way strategy touches the world. `FakeRobotIO` — simulated robot for tests and bench |

**Strategy** — pure C++, must not include `Arduino.h`

| Module | Status | Contents |
|---|---|---|
| `maze/` | built | `MazeSolver` — the shared runner: belief map, pose, move log, red-tile safety net, finishing phase. `ExplorationStrategy` — interface for "which way next?". Strategies: `WallFollower`, `DfsExplorer`, `DfsStraightFirst`, `FrontierExplorer`. `PathPlanner` — BFS over the known map |
| `levels/` | planned | `BallSearch`, `GapRunner`, `ColorPath` — pista B, three small state machines, no map |

**Coordination**

| Module | Status | Contents |
|---|---|---|
| `robot/` | planned | `RobotStateMachine` (mode, deadline, restart), `RealRobotIO` (hardware-backed `IRobotIO`) |
| `safety/` | planned | `SafetySupervisor` — can cut motor output regardless of what strategy requests |

**Subsystems** — hardware-facing

| Module | Status | Contents |
|---|---|---|
| `drivetrain/` | planned | advance one unit, turn 90°/180°, strafe, stop; closed-loop control |
| `odometry/` | planned | pose, heading, confidence; ramp pitch projection |
| `perception/` | planned | ToF → walls, front color sensor → tile ahead, IR → white line |
| `capture/` | planned | gripper: open, close, ball-held check |
| `indicator/` | planned | OLED: detected color, ArUco ID, status |
| `vision_link/` | planned | UART to the Pi — parsing, heartbeat freshness |
| `persistence/` | planned | `MissionState` in NVS, survives the power cycle after a lack of progress |
| `hal/` | planned | one thin wrapper per chip |

**Test tooling** — never compiled for the ESP32

| Module | Status | Contents |
|---|---|---|
| `maze_gen/` | built | `MazeGenerator` (seeded random 5×5 mazes with loops, corner start and red), `MazeValidator` (independent checker) |

### `vision/`

| File | Purpose |
|---|---|
| `main.py` | capture loop |
| `aruco_detector.py` | OpenCV ArUco detection |
| `ball_detector.py` | detect the ball and report its image position |
| `serial_link.py` | UART to the ESP32 — receive mode commands; send detections, heartbeat, errors |
| `calibration/` | camera intrinsics |
| `systemd/` | service file so vision starts on boot |

### `tools/`

| File | Purpose |
|---|---|
| `bench_report.py` | reads the benchmark CSV, produces tables and plots |
| `telemetry_listener.py` | receives UDP telemetry from the robot |

---

## Layering rules

1. **Dependencies point downward only.** `Perception` reports what it sees; it
   never commands the `Drivetrain`. The one exception is `SafetySupervisor`,
   which gates motor output directly, because routing an emergency stop up and
   back down costs latency where latency matters most.
2. **Strategy modules must not include `Arduino.h`.** They reach the world
   through `IRobotIO`. This is what makes `pio test -e native` possible.
3. **`src/` holds only `main.cpp`.** Everything else lives in a `lib/` module.
4. **Test tooling stays off the robot.** `maze_gen/` and `bench/` are only
   built by the `native` and `bench` environments.

---

## Building

```bash
cd firmware

pio run -e esp32               # build for the ESP32-S3
pio run -e esp32 -t upload
pio device monitor

pio test -e native             # all unit tests, on the laptop
pio test -e native -f test_strategies   # one suite

pio run -e bench && .pio/build/bench/program > ../tools/results.csv
```
