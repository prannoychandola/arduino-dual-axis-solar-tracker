# Arduino Uno Dual-Axis Solar Tracker

An Arduino Uno project that detects the direction of sunlight and rotates a solar panel toward the brighter direction on both the horizontal and vertical axes.

## How the tracking logic works

The sketch reads four directional light values and averages them in pairs:

- Left side versus right side determines the horizontal correction.
- Top side versus bottom side determines the vertical correction.

When the difference on an axis is greater than the configured deadband, the corresponding servo moves by one small step toward the brighter side. Small differences are ignored to reduce jitter. The angle limits prevent the servos from moving beyond the configured range.

The two axes are adjusted independently, so the panel can correct its horizontal and vertical direction during the same update.

## Sketch

Open [SolarTracker.ino](SolarTracker.ino) in the Arduino IDE, select **Arduino Uno**, and upload the sketch.

The default sketch pin mapping is:

- Directional light readings: A0-A3
- Horizontal and vertical servo signals: D9 and D10

Change the pin constants near the top of the sketch if your setup uses different pins. The code uses Arduino's built-in `Servo.h` library.

## Tuning

- Set `LIGHT_DEADBAND` higher if the panel jitters around a centered position; lower it if it does not react to small light differences.
- Set `REVERSE_AZIMUTH` or `REVERSE_ELEVATION` to `true` if an axis moves away from the brighter direction.
- Adjust `MIN_ANGLE`, `MAX_ANGLE`, and `START_ANGLE` to suit the panel's movement range and starting position.
- Increase `ANGLE_STEP` for faster, larger corrections; decrease it for finer movement.
- Open the Serial Monitor at 9600 baud to view the directional readings and current angles while tuning.

## Tracking cycle

```text
Read directional light values
          ↓
Compare left/right and top/bottom averages
          ↓
Ignore differences within the deadband
          ↓
Move each axis one step toward its brighter side
          ↓
Repeat
```

## Project

This is a learning project demonstrating Arduino-based sunlight detection and dual-axis tracking logic.
