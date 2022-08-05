# `feedback360`

This is an Arduino library for reading the position of a Parallax Feedback 360 servo.

`map()` is also redefined as a preprocessor macro so that it can accept floating point numbers. The math is the same as the original `map()` and there should actually be a tiny performance boost.

The data sheet from Parallax is included in the `extras/` folder.

## Documentation

### Constructor `Feedback360(Servo *s, uint8_t fbPin)`

`Servo` is a reference to an instance of the standard Servo class, which should be `attach()`'ed to the white wire.

`fbPin` is the pin number of the yellow wire, the feedback signal, which should be setup as an `INPUT`.

### `void Feedback360::readPosition()`

Call this to read the encoder and update the position variables.

### `double Feedback360::theta`

Absolute position in fractions of a turn from zero.

### `int32_t Feedback360::turns`

Cumulative number of turns since the program started.

### `double Feedback360::speed`

Current angular velocity in turns per second.

### `void Feedback360::setPower(double power)`

Sets the power applied to the motor using the provided `Servo` instance, mapping the input range of -1.0 to 1.0 to the 1280-1720&micro;s range that the Feedback 360 servo responds to (as opposed to the usual 1000-2000&micro;s that most other servos use).

## Notes

* 1\. Parallax says you should call `readPosition()` between every servo pulse (50Hz); however in my experimentation I have found it's only necessary to call it four times per rotation (one in each quadrant); and as it's an absolute encoder, if you only need the angular position and don't care about speed or cumulative turns count, you only need to call `readPosition()` whenever you actually need the position.

* 2\. On my servo, `setPower(1.0)` caused the motor to rotate backwards (towards negative position values) as reported by the encoder, and vice versa. Do keep this in mind if you are using a PID control loop with this library.

* 3\. Unlike other servos, in the Feedback 360 (or at least the one I tested on) there is a short delay between when the PWM value is changed by the microcontroller and when the motor actually responds to the change, so direct PID control of position may be difficult.
