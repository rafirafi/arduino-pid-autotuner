# arduino-pid-autotuner
Automated PID tuning using Ziegler-Nichols/relay method.

Originally written for Arduino and compatible boards, but does not rely on the Arduino standard library.

## How does it work?
`pidautotuner.h` and `pidautotuner.cpp` are fully commented to explain how the algorithm works.

## What PID controller does this work with?
This algorithm should work with any implementation of PID control if it is properly configured.

## Example code (Arduino)
```c
#include <pidautotuner.h>

void setup() {

    PIDAutotuner tuner = PIDAutotuner();

    // Set the target value to tune to
    // This will depend on what you are tuning. This should be set to a value within
    // the usual range of the setpoint. For low-inertia systems, values at the lower
    // end of this range usually give better results. For anything else, start with a
    // value at the middle of the range.
    tuner.setTargetInputValue(targetInputValue);

    // Set the output range
    // These are the minimum and maximum possible output values of whatever you are
    // using to control the system (Arduino analogWrite, for example, is 0-255)
    tuner.setOutputRange(0, 255);

    // Set the Ziegler-Nichols tuning mode
    // Set it to either PIDAutotuner::ZNModeBasicPID, PIDAutotuner::ZNModeLessOvershoot,
    // or PIDAutotuner::ZNModeNoOvershoot. Defaults to ZNModeNoOvershoot as it is the
    // safest option.
    tuner.setZNMode(PIDAutotuner::ZNModeBasicPID);

    // This must be called immediately before the tuning loop
    // Must be called with the current time in microseconds
    tuner.startTuningLoop(micros());

    // Run a loop until tuner.isFinished() returns true
    long microseconds;
    while (!tuner.isFinished()) {

        long prevMicroseconds = microseconds;
        microseconds = micros();

        // Get input value here (temperature, encoder position, velocity, etc)
        double input = doSomethingToGetInput();

        // Call tunePID() with the input value and current time in microseconds
        double output = tuner.tunePID(input, microseconds);

        // Set the output - tunePid() will return values within the range configured
        // by setOutputRange(). Don't change the value or the tuning results will be
        // incorrect.
        doSomethingToSetOutput(output);

        while (micros() - microseconds < loopInterval) delayMicroseconds(1);
    }

    // Turn the output off here.
    doSomethingToSetOutput(0);

    // Get PID gains - set your PID controller's gains to these
    double kp = tuner.getKp();
    double ki = tuner.getKi();
    double kd = tuner.getKd();
}

void loop() {

    // ...
}
```
