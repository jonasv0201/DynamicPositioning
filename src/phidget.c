#include "main.h"
#include "phidgetHelper.c"


int setUpPhidgets(PhidgetVoltageInputHandle potHandle, PhidgetRCServoHandle motorHandle) {
    /* Sets up the connections with the potensiometer and the motor (Phidget devices).
     * Returns 0 if connections are successful, otherwise 1.
     * Prints relevant messages to the screen.
     */

	PhidgetReturnCode prc;

	//Set Potensiometer channel and serial number
	Phidget_setDeviceSerialNumber((PhidgetHandle) potHandle, 76241);
	Phidget_setChannel((PhidgetHandle) potHandle, 0);

	//Set event handlers for potensimeter and motor
	if (setHandlers(potHandle, motorHandle) ){
        goto error;
	}

	printf("Waiting for attach reply from potensiometer...\n");
	prc = Phidget_openWaitForAttachment((PhidgetHandle) potHandle, 5000);
	if (prc != EPHIDGET_OK) {
		if (prc == EPHIDGET_TIMEOUT) {
			printf("Potensiometer channel did not attach after 5 seconds: please check that the device is attached\n");
		}
        goto error;
	}

    printf("Waiting for attach reply from motor...\n");
	prc = Phidget_openWaitForAttachment((PhidgetHandle)motorHandle, 5000);
	if (prc != EPHIDGET_OK) {
		if (prc == EPHIDGET_TIMEOUT) {
			printf("Motor channel did not attach after 5 seconds: please check that the device is attached\n");
		}
        goto error;
	}

	printf("Setting motor target thrust to 100 and engaging motor\n");
	prc = PhidgetRCServo_setTargetPosition((PhidgetRCServoHandle) motorHandle, 100);
	if (prc != EPHIDGET_OK) {
		printf("Failed to set motor target thrust\n");
	}
	prc = PhidgetRCServo_setEngaged((PhidgetRCServoHandle) motorHandle, 1);
	if (prc != EPHIDGET_OK) {
		printf("Failed to engage motor\n");
		goto error;
	}

	printf("Connected to Phidgets successfully!\n");
	return 0;


	//If setup is unsuccessful, close device connections safely
	error:
    closePhidgets(motorHandle, potHandle);
    return 1;
}

float getPosition(PhidgetVoltageInputHandle pot) {
    /*Returns the position of the boat in cm */

	//Get current voltage from potensiometer. Returns max voltage(=5) when boat is on the back end.
	PhidgetVoltageInput_getSensorValue(pot, &voltage);

	//Return position in cm. Equation units: [cm -  V *(cm/V)]
	return (float) ( (maxPos) - (voltage - minVolt) * (maxPos / (maxVolt - minVolt)) );
}

void setMotorThrust(PhidgetRCServoHandle motorHandle, float thrust){
	/*Sets new target position (thrust) for the motor*/

	PhidgetRCServo_setTargetPosition(motorHandle , thrust);
}

void closePhidgets(PhidgetRCServoHandle motorHandle, PhidgetVoltageInputHandle potHandle){
    /* Closes connections with the phidgets safely*/

    //Shutting motor off and closing the channel
    PhidgetRCServo_setEngaged((PhidgetRCServoHandle) motorHandle, 0);
	Phidget_close((PhidgetHandle)motorHandle);
	PhidgetRCServo_delete(&motorHandle);

    //Closing the potensiometer channel
    Phidget_close((PhidgetHandle)potHandle);
    PhidgetVoltageInput_delete(&potHandle);
}
