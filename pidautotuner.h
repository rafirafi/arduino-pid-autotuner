// PID automated tuning (Ziegler-Nichols/relay method) for Arduino and compatible boards
// Copyright (c) 2016-2020 jackw01
// This code is distrubuted under the MIT License, see LICENSE for details

#ifndef PIDAUTOTUNER_H
#define PIDAUTOTUNER_H

class PIDAutotuner {
  public:
    // Constants for Ziegler-Nichols tuning mode
    typedef enum {
      ZNModeBasicPID,
      ZNModeLessOvershoot,
      ZNModeNoOvershoot
    } ZNMode;

    PIDAutotuner();

    // Configure parameters for PID tuning
    // See README for more details
    // targetInputValue: the target value to tune to
    // outputRange: min and max values of the output that can be used to control the system (0, 255 for analogWrite)
    // znMode: Ziegler-Nichols tuning mode (znModeBasicPID, znModeLessOvershoot, znModeNoOvershoot)
    // tuningCycles: number of cycles that the tuning runs for (optional, default is 10)
    void setTargetInputValue(float target);
    void setOutputRange(float min, float max);
    void setZNMode(ZNMode zn);
    void setTuningCycles(int tuneCycles);

    // Must be called immediately before the tuning loop starts
    void startTuningLoop(unsigned long us);

    // Automatically tune PID
    // See README for more details
    float tunePID(float input, unsigned long us);

    // Get results of most recent tuning
    float getKp();
    float getKi();
    float getKd();

    bool isFinished(); // Is the tuning finished?

    int getCycle(); // return tuning cycle

  private:
    float targetInputValue = 0;
    float minOutput, maxOutput;
    ZNMode znMode = ZNModeNoOvershoot;
    int cycles = 10;

    // See startTuningLoop()
    int i;
    bool output;
    float outputValue;
    long t1, t2, tHigh, tLow;
    float max, min;
    float pAverage, iAverage, dAverage;

    float kp, ki, kd;
};

#endif
