/*dynamic/src___________________________________________________________________
Name: main.c
________________________________________________________________________________
Purpose: The program will ask the user for a desired target position[cm] of the
          boat and then it will use a PID-algorithm to maintain that position.
          Parameters as position, input (and its components), error and set point
          will be logged and plotted with Gnuplot. Logs and plots are saved in
          dynamic/logs.

Parameters:
Check main.h

Subroutines & location:

    newSetPoint()           mainSub.c
    getUnixTime()           mainSub.c
    adjustValue()           mainSub.c
    gnuPipe()               gnuplot.c
    setUpPhidets()          phidget.c
    getPosition()           phidget.c
    closePhidgets()         phidget.c
    setMotorThrust()        phidget.c
_____________________________________________________________________________
Made by student: 69545
________________________________________________________________________________*/

#include "main.h"

int main(int argc, char* argv[]) {

    //Ask user for new target position for boat (Set Point is redefined)
    newSetPoint();

    //__________Logging file__________
    //Create dat-file for logging
    char fileName[32];
    sprintf (fileName, "../logs/log%i.dat", fileNum);
    printf("Logs are saved in file: %s\n", fileName);
    datFile = fopen(fileName, "w");


    //__________Connect to Phidgets__________

    //Declare and Create Potensiometer handle for measuring of position
    PhidgetVoltageInputHandle potHandle = NULL;
    PhidgetVoltageInput_create(&potHandle);

    //Declare and Create Motor handle for setting motor speed
    PhidgetRCServoHandle motorHandle = NULL;
    PhidgetRCServo_create(&motorHandle);

    //Connect to the potensiometer and motor
    if (setUpPhidgets(potHandle, motorHandle)){
        printf("Problems with connecting to Phidgets. Program is terminating");
        return 1;
    }


    //__________Prepare for control loop__________

    //Gives smaller dTerm in first iteration -> Smaller derivative kick
    lastError = 20;

    startPos =  getPosition (potHandle);
    startTime_sec = getUnixTime();

    //__________Control loop__________
    do {
        //__________PID Algorithm__________

        //Get measured current position of boat
        pos = getPosition(potHandle);

        //Set error
        error = (float) setPoint - pos;

        //Calculate the integral-term and check that it is not too big or small
        iTerm +=  Ki*error*loopIteration_ms/1000;

        //Avoid extreme values
        adjustValue(&iTerm, 80, 0, "iTerm");

        //Calculate the derivative-term and limit its impact if necessary
        dTerm = (float) Kd * ( (error - lastError) *1000/ loopIteration_ms );

        //Avoid extreme values
        adjustValue(&dTerm, 20, -20, "dTerm");

        //Sum up the P, D, I and offSetMotor terms
        output = (float) Kp*error + iTerm + dTerm + offSetMotor;

        //Adjust for saturation
        adjustValue(&output, outMax, outMin, "output");

        //Set new motor thrust
        setMotorThrust(motorHandle , output);

        //__________Logging and preparation for next loop__________

        //Print output and the terms to screen
        printf("Output = %f + %2f + %2f = %f\tDeviation: %f\n", Kp*error, iTerm, dTerm, output, error);

        //Print to logging file
        //Format: time[s], position[cm], prc-error, output, error[cm], proportional, integral, derivative, OffsetMotor
        fprintf(datFile, "%f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%i\n",getUnixTime()-startTime_sec, pos, 100*(error/(startPos-setPoint)), output, error,Kp*error, iTerm,dTerm,offSetMotor);

        //Prepare for next loop
        lastError = error;

        //Sleep for loopIteration_ms milli seconds
        usleep(loopIteration_ms *1000);

        //Check if the loop should break
    } while(endLoopTime_sec > getUnixTime()-startTime_sec);

    printf("PID Control Loop Ended\n");

    //Safely end the connection with the potensiometer and motor
    closePhidgets(motorHandle,potHandle);

    //Close logging file
    fclose(datFile);

    //Generate plots with Gnuplot
    gnuPipe(fileName);

    //Adding execution info to logging file after the gnuplots have been created
    datFile = fopen(fileName, "a");
    fprintf(datFile, "Execution Info: \nSet Point: %i\tIteration time: %i\nKp: %f\tKi: %f\tKd: %f\t\n",(int) setPoint,(int)loopIteration_ms,Kp,Ki,Kd);
    fclose(datFile);

    return 0;

}
