# `feedback360`

This is an Arduino library for reading the position of a Parallax Feedback 360 servo.

`map()` is also redefined as a preprocessor macro so that it can accept floating point numbers. The math is the same as the original `map()` and there should actually be a tiny performance boost.

The data sheet from Parallax is included in the `extras/` folder.

## Documentation

### Constructor `Feedback360(boolean reversed = true)`

`reversed` is a flag to indicate if making the internal servo go "forward" causes the encoder to report "backwards" rotation.

### `void Feedback360::attach(uint8_t controlPin, uint8_t feedbackPin)`

`controlPin` is the white wire, `feedbackPin` is the yellow wire.

### `void Feedback360::detach()`

Detaches the internal `Servo` instance used to control the motor.

### `boolean Feedback360::readPosition()`

Call this to read the encoder and update the position variables. As this library doesn't use interrupts, this call will block for the time of one cycle, usually around 1100&micro;s, but can be as long as 2300&micro;s on occasion, and a maximum of 20ms (20000&micro;s) if the yellow wire got disconnected and `pulseIn` timed out. Returns true if position was successfully read; false if not.

### `double Feedback360::theta`

Absolute angular position in fractions of a turn from zero. Always positive (i.e. $0\leq\text{theta}\lt 1$). Because it's an absolute encoder,`theta` cannot be changed to set a "virtual home position" other than the hardware defined zero. If you are interested, an issue or pull request would be welcome.

### `int32_t Feedback360::turns`

Cumulative number of whole turns since the program started. You can write to this to "reset" the turns count, but be warned that doing this will cause a huge single-frame spike or dip in `speed` which may confuse an attached PID controller when regulating speed.

### `double Feedback360::position`

A convenience for accessing the full position (theta + turns). If you want to "reset" the turns count, write to `turns` instead, and this will automatically update.

### `double Feedback360::speed`

Current angular velocity in turns per second.

### `void Feedback360::setPower(double power)`

Sets the power applied to the motor using the provided `Servo` instance, mapping the input range of -1.0 to 1.0 to the 1280-1720&micro;s range that the Feedback 360 servo responds to (as opposed to the usual 1000-2000&micro;s that most other servos use). if `reversed` is true then the range is reversed.

## Notes

* 1\. Parallax says you should call `readPosition()` between every servo pulse (50Hz); however in my experimentation I have found it's only necessary to call it four times per rotation (one in each quadrant); and as it's an absolute encoder, if you only need the angular position and don't care about speed or cumulative turns count, you only need to call `readPosition()` whenever you actually need the position.

* 2\. On my servo, `setPower(1.0)` with `reversed = false` caused the motor to rotate backwards (towards negative position values) as reported by the encoder, and vice versa. Do keep this in mind if you are using a PID control loop with this library. This is why `reversed` defaults to true.

* 3\. Unlike other servos, in the Feedback 360 (or at least the one I tested on) there is a short delay between when the PWM value is changed by the microcontroller and when the motor actually responds to the change, so direct PID control of position was quite difficult. (Speed control was easier.)
