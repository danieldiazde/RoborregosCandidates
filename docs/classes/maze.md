# Maze modules

Status: **implemented in pure C++; physical behavior unverified**.

`MazeSolver` owns the current pose, belief map, bounded move log, exploration
strategy, and transition into finishing. `WallFollower`, `DfsExplorer`,
`DfsStraightFirst`, and `FrontierExplorer` choose a next absolute direction.
`PathPlanner` uses BFS on known-open edges.

Example: at `(0,0)` facing North, the solver records valid left/front/right wall
readings in its belief, asks the strategy for a direction, turns through
`IRobotIO`, samples the tile ahead, and advances only after a successful action.
During exploration it refuses a known red tile. Finishing may enter red.

## Invariants and limitations

- Local start is `(0,0)`, North, boundary-left by team convention.
- Belief is distinct from fake world truth; unknown walls remain unknown and are
  never passable.
- A correct strategy should request only known-open moves, so `blockedCount`
  remains zero in the fake. A blocked result indicates changed truth, bad sensing,
  or an invalid assumption and must not advance the solver pose.
- DFS currently updates its stack while proposing a move and repairs it on a
  later call. Keeping the DFS top equal to the actual robot cell after every
  outcome is a Phase 1 invariant, not a tested guarantee of this implementation.
- Storage is fixed. The current 256-entry log can become incomplete before the
  500-step loop limit; exact replay is therefore not promised.
- `run()` does not fully reset solver/strategy state for reuse. Action failures,
  finish-cost estimates, score events, and optional post-red return need later work.
- The 60-second threshold and fake motion costs are defaults, not measurements.

Native grid, strategy, path-planner, and generator suites cover current pure
logic. They do not demonstrate sensors, movement, terrain, timing, or scoring.

## How to explain this to a mentor

The solver is the referee for one run; a strategy only proposes directions. The
solver owns sensing, red protection, pose changes, and the belief map so every
strategy obeys the same world boundary and safety rule.
