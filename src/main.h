/*dynamic/src___________________________________________________________________
Name: main.h
________________________________________________________________________________
Purpose:  main.h defines constants, declares variables and includes libraries for all
          subroutines except for phidgetHelper.c, which is included directly to phidget.c


____________________________________________________________________________________
_________________________________Parameters________________________________________

__Name__                __Type__              __Description__
setPoint                macro               Target position for the vessel.
Kp                      macro               Proportional gain constant.
Ki                      macro               Integrative gain constant.
Kd                      macro               Derivative gain constant.
endLoopTime_sec         macro               Duration of the loop[sec].
loopIteration_ms        macro               Time between PID iterations.

fileNum                 macro               Number to identify logs and plots.

outMax                  macro               Max thrust to motor before saturation.
outMin                  macro               Min thrust to avoid stopped propellers.
offSetMotor             macro               Offset to adjust the motors shifted input.

maxPos                  macro               Max length the boat can travel. (67 cm)
maxVolt                 macro               Max voltage from potensiometer. 5V when pos=0.
minVolt                 macro               Min voltage from potensiometer. ca 3.6V, pos=67.

pos                     float               Measured position of the vessel.
startPos                float               Start position of the vessel.
error                   float               Deviation between pos and setPoint.
lastError               float               Error in last PID computation.
output                  float               Output from PID algorithm. New thrust to motor.
iTerm                   float               Integrative component of PID.
dTerm                   float               Derivative component of PID.

startTime_sec           double              The time before the loop begins.

datFile                 FILE*               File pointer to log file.
fileName                Char[32]            Where the log is stored. --> /logs/log[fileNum].dat

voltage                 double              The voltage measured by the potensiometer.

potHandle               PhidgetVoltageInputHandle       Phidget handle for potensiometer.
motorHandle             PhidgetRCServoHandle            Phidget handle for servo motor.


Subroutines & location:

    newSetPoint()           mainSub.c
    getUnixTime()           mainSub.c
    adjustValue()           mainSub.c
    gnuPipe()               gnuplot.c
    setUpPhidets()          phidget.c
    getPosition()           phidget.c
    closePhidgets()         phidget.c
    setMotorThrust()        phidget.c
________________________________________________________________________________*/

#ifndef SRC_MAIN_H
#define SRC_MAIN_H


//__________Include libraries__________
#include <stdio.h>
#include <phidget22.h>
#include <time.h>
#include <unistd.h>


//Obs: phidget.c includes phidgetHelper.h and its subroutines

//__________Define constant variables__________

//PID related variables
#define setPoint 35
#define Kp 1.0
#define Ki 0.18
#define Kd 0.1
#define endLoopTime_sec 15
#define loopIteration_ms 30

//File number for plots and logs
#define fileNum 17

//Thrust limits for motor: [0,66] -> Reverse and [71, ~180] -> Forward
#define outMax 160
#define outMin 85
#define offSetMotor 72

//Potensiometer constants
#define maxPos 67
#define maxVolt 5
#define minVolt 3.6


//__________Variables__________

//PID variables
float pos, startPos;
float error, lastError;
float output, dTerm, iTerm;
double startTime_sec;

//Logging variable
FILE  *datFile;

//Potensiometer variable
double voltage;

//__________Subroutines__________

//Phidget subroutines used directly by main
float getPosition(PhidgetVoltageInputHandle pot);
int setUpPhidgets(PhidgetVoltageInputHandle potHandle, PhidgetRCServoHandle motorHandle);
void closePhidgets(PhidgetRCServoHandle motorHandle, PhidgetVoltageInputHandle potHandle);
void setMotorThrust(PhidgetRCServoHandle motorHandle, float output);

//Other subroutines
void newSetPoint();
void gnuPipe(const char *fileName);
double getUnixTime();
void adjustValue(float *value, int max, int min, char name[16]);

/* OBS: phidgetHelper.c is included directly to phidget.c */


#endif //SRC_MAIN_H
