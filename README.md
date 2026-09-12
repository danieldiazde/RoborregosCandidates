# RoBorregos Candidates 2026 - Slash Ultrathink

- Controller: ESP32-S3, C++ / Arduino framework / FreeRTOS
- Vision: Raspberry Pi Zero 2 W, Python + OpenCV, linked over UART

See [`docs/architecture.md`](docs/architecture.md) for layered design.

---

## Repo Layout

```
docs/         design documents
firmware/     everything running on the ESP32
vision/       Python running on the Raspberry Pi
tools/        laptop-side helpers
```

### `docs/`

| Path | Contents |
|---|---|
| `architecture.md` | the layered design |
| `architecture.drawio.svg` | the diagram |
| `states.md` | robot state machine, including checkpoint restart |
| `uart.md` | ESP32 ↔ Pi message table and failure handling |
| `pinout.md` | GPIO map |
| `classes/` | one class diagram per firmware module |
| `adr/` | architecture decision records |

### `firmware/`

PlatformIO project. Open `firmware/` as the workspace root, or open
`candidates.code-workspace` from the repo root to get firmware, vision and
docs in one VS Code window.

```
platformio.ini      build configuration
src/main.cpp        entry point — creates FreeRTOS tasks
lib/<module>/       one folder per module, .hpp and .cpp together
test/               native unit tests
```

Modules:

**Foundation** 

| Module | Contents |
|---|---|
| `types/` | `Direction` and its turn arithmetic, `Observation`, `Action`, `Status` |
| `robot_io/` | `IRobotIO` (the interface strategy calls instead of hardware), `FakeRobotIO` (simulated maze used by the native tests) |

**Strategy**

| Module | Contents |
|---|---|
| `maze/` | `Maze` (5×5 grid, edge-indexed walls), `Cell`, `MazeSolver` (DFS and exploration stack) |
| `levels/` | `BallSearch`, `GapRunner`, `ColorPath` — three small state machines |

**Coordination**

| Module | Contents |
|---|---|
| `robot/` | `RobotStateMachine` (mode, deadline, restart recovery), `RealRobotIO` (hardware-backed implementation of `IRobotIO`) |
| `safety/` | `SafetySupervisor` — can cut motor output regardless of what strategy requests |

**Subsystems**

| Module | Contents |
|---|---|
| `drivetrain/` | advance a specified distance, turn to a heading, stop; closed-loop control |
| `odometry/` | pose, heading, confidence. Ramp pitch projection |
| `perception/` | ToF → walls, color sensor → tile color, IR → white line |
| `capture/` | gripper: open, close, ball-held check |
| `indicator/` | OLED: detected color, ArUco ID, status |
| `vision_link/` | UART to the Pi — parsing, heartbeat freshness |
| `persistence/` | `MissionState` in NVS, survives the power cycle after a lack of progress |
| `hal/` | one thin wrapper per chip. HAL isolates device-specific code |
| `track_log/` | 

### `vision/`

| File | Purpose |
|---|---|
| `main.py` | capture loop |
| `aruco_detector.py` | OpenCV ArUco detection |
| `ball_detector.py` | Detect the ball and report its image position |
| `serial_link.py` | UART output to the ESP32 - Receive mode commands; send detections, heartbeat and errors |
| `config.yaml` | resolution, serial port, thresholds |
| `calibration/` | camera intrinsics |
| `systemd/` | service file so vision starts on boot |

### `tools/`

| File | Purpose |
|---|---|
| `telemetry_listener.py` | receives UDP telemetry from the robot |
| `maze_sim.py` | runs the solver against a simulated maze with injectable odometry drift |

---

## Layering rules

1. **Dependencies point downward only.** `Perception` reports what it sees;
   it never commands the `Drivetrain`. The one exception is
   `SafetySupervisor`, which gates motor output directly — that path exists
   because routing an emergency stop up and back down costs latency where
   latency matters most.
2. **Strategy modules must not include `Arduino.h`.** They reach the world
   through `IRobotIO`. This is what makes `pio test -e native` possible, and
   it is the single constraint the whole architecture exists to satisfy.
3. **`src/` holds only `main.cpp`.** The `main.cpp` is focused on initialization and
   task setup. Reusable modules under `lib/`.

---

## Building

```bash
cd firmware

pio run -e esp32          # build for the ESP32-S3
pio run -e esp32 -t upload
pio device monitor

pio test -e native        # run the strategy tests on laptop
```

The `native` environment builds only the pure modules, so the maze logic can
be tested without the robot.

---