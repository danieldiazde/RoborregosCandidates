# ADR 0001: Isolate native test support from production firmware

- Status: accepted in Phase 0
- Date: 2026-10-05

## Context

`FakeRobotIO` originally lived beside the production `IRobotIO` contract. That
allowed dependency discovery to pull a simulated truth map into an ESP32 build
and made a successful link weak evidence of production coverage.

## Decision

Keep `IRobotIO` in production `robot_io`. Put `FakeRobotIO` and its simulated
timing/world in `robot_io_fake`, a native-only library used by tests and
benchmarks. Keep `maze_gen` and benchmark sources native-only. The ESP32 build
compiles the production path and excludes test support.

## Alternatives considered

Keep the fake in `robot_io` and compile it conditionally only for native targets.
This would avoid another library, but production and test responsibilities would
remain mixed and a missing build condition could silently expose fake truth to
the controller. A separate native-only library makes the boundary visible in its
manifest and independently checkable in the ESP32 dependency graph, so Phase 0
chooses separation.

## Consequences

Production modules cannot depend on fake truth. Native tests retain a simple,
deterministic adapter, while ESP32 dependency inspection can verify that neither
fake nor generator ships. New fakes implement the same pure interface and remain
native-only.

## How to explain this to a mentor

The interface is a socket shape; the real robot and simulator are two plugs.
Shipping the simulator would add an imaginary world to the robot, so build
configuration permits that plug only on the laptop.
