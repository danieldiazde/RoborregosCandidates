#pragma once

#include <cstdint>

namespace config {

// TODO(team): replace these unset values only after selecting and verifying hardware.
// Code that drives hardware must reject kUnsetGpio and must default to outputs off.
inline constexpr std::int8_t kUnsetGpio = -1;
inline constexpr bool kHardwareConfigurationVerified = false;

// Rule and geometry constant for the fixed competition maze.
inline constexpr std::int8_t kMazeSize = 5;

// Fixed solver storage and loop limits preserved from the Phase 0 baseline.
inline constexpr std::uint16_t kMaxMoveLogEntries = 256;
inline constexpr std::uint16_t kMaxSolverSteps = 500;

// Simulation defaults preserved from the Phase 0 baseline. They are not measurements.
inline constexpr std::uint32_t kSimulatedRoundDurationMs = 360000;
inline constexpr std::uint32_t kSimulatedFinishReserveMs = 60000;
inline constexpr std::uint32_t kSimulatedAdvanceMs = 3000;
inline constexpr std::uint32_t kSimulatedStrafeMs = 3500;
inline constexpr std::uint32_t kSimulatedTurnMs = 1500;
inline constexpr std::uint32_t kSimulatedTurnAroundMs = 2500;

}  // namespace config
