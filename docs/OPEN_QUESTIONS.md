# Open questions

Last reviewed October 5, 2026. Tournament: **October 17, 2026**.

No devices are connected. The exact ESP32, Raspberry Pi, camera, display,
sensors, motors, motor drivers, encoders, chassis dimensions, wheel geometry,
power system, reset circuit, and pin allocation are unconfirmed. The Raspberry
Pi model, OS image/architecture, camera API, and display model are also
unconfirmed; repository names and build targets are not hardware evidence.

Until these facts are answered, `config.h` keeps affected values unset and
bring-up code must refuse to energize outputs.

## Team and hardware

- Which controller and Pi are physically installed? Does restart reset only the
  ESP32 or power-cycle the whole robot? This restart/power scheme is explicitly
  deferred as a hardware decision.
- Which pins, logic voltages, buses, addresses, motor polarity, encoder counts,
  wheel dimensions, sensor ranges, and safe speed/current limits apply?
- Which camera, lens, mount, lighting range, display, and Pi OS/interpreter will
  be used? Can judges see the displayed marker while the robot moves?
- How are current-tile color, tile-ahead color, walls, lines, terrain,
  checkpoint occupancy, ball location, and greater-than-half ball possession
  physically sensed? Sensor reach and the exact repeatable placement fixture are
  unknown.

## Rules and recovery

- Are corner start/red placement and boundary-left starting orientation
  guaranteed? They are currently preserved as team conventions.
- May a map learned during the current round survive a restart? Map persistence
  is undecided; restore is disabled until judges confirm it.
- How does the captain declare the optional fourth-restart section skip, and
  what robot input is permitted? Section skip is undecided and must not award
  unobserved checkpoint points.
- What trusted time source survives the chosen reset/power scheme? Recovery must
  preserve total elapsed round time. If elapsed time cannot be established,
  resuming a scored run is blocked.
- For Pista B, does the final 85-point award require uninterrupted possession
  after a drop/restart, or possession at scoring time? Use the stricter reading
  until judges answer.
- Section 5 says a Section 1 reset returns the ball to its enclosure, while
  Sections 2 and 3 place it in front of the robot. What exact distance/alignment
  and placement procedure will judges use, and what state may the robot retain?
  Do not invent pose, ball state, or completion evidence.

Owners and dates should be added when assigned. Answers that affect architecture,
safety, or rule interpretation require an ADR and tests before activation.
