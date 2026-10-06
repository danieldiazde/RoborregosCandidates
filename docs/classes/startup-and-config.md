# Startup and configuration

Status: **Phase 0 foundation only**.

Hardware status: **UNVERIFIED ON HARDWARE**. No device is connected.

`firmware/src/main.cpp` contains inert Arduino `setup()` and `loop()` functions
so the target links. They create no tasks, touch no pins, and energize no output.
This is intentional while hardware is unknown.

`firmware/include/config.h` centralizes firmware constants. Phase 0 may contain
software-only limits and simulated timings, but hardware pins and measured
calibration values must stay explicitly unset until the team confirms them.
Later bring-up programs must reject unresolved output configuration before
driving hardware.

Example: adding a motor driver requires first recording the actual driver,
voltage, pins, polarity, and safe limits; then adding a narrow bring-up target.
Putting a guessed pin in `main.cpp` breaks this boundary.

## Tests and hardware status

The ESP32 build can prove that the inert entry point and production libraries
compile and link. It cannot prove board identity, boot, wiring, or output safety.
No device is connected during Phase 0.

## How to explain this to a mentor

The empty runtime is a deliberate fuse: the team can establish a reproducible
build without risking an unknown motor pin. Configuration becomes real only when
it is tied to inspected hardware and a controlled bring-up check.
