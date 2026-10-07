# Pista B levels

Status: **planned / unfinished**.

`ColorPath.hpp` is a placeholder. `BallSearch`, `GapRunner`, color-path following,
checkpoint recovery, ball placement, possession scoring, and section skipping do not exist.
No hardware or simulation result supports those behaviors yet.

Phase 2 builds the strategies against a simulated world without connected
hardware. Physical integration and unresolved recovery policies depend on the
answers in [`../OPEN_QUESTIONS.md`](../OPEN_QUESTIONS.md). Do not award a
checkpoint or possession score without an observed completion event.

## How to explain this to a mentor

The folder reserves a home for three small Pista B state machines. It is a label,
not working software. Showing the placeholder honestly is better than presenting
future class names as completed engineering.
