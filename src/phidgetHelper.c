#include "main.h"

/* Change handlers that are copied and slightly changed from Phidget homepage.
 *
 * CCONV is a defined "Calling Convention" for Phidget library.
 *
 * Motor Handler: OnPositionChangeHandler
 *
 * Potensiometer Handler: OnVoltageChangeHandler --> PotInterval set to 20
 *
 * Both: OnAttach-, OnDetach-, OnErrorhandler
 */

static void CCONV onAttachHandler_pot(PhidgetHandle ph, void *ctx) {
PhidgetReturnCode prc;
int32_t serialNumber;
PhidgetHandle hub;
int32_t hubPort;
int32_t channel = 0;
uint32_t potInterval_ms = 20;

printf("\tSetting potensiometers data interval to %i\n", potInterval_ms);
prc = PhidgetVoltageInput_setDataInterval((PhidgetVoltageInputHandle)ph, potInterval_ms);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Set DataInterval: \n\t");
displayError(prc);
return;
}

printf("\tSetting Voltage ChangeTrigger to 0.0\n");
prc = PhidgetVoltageInput_setVoltageChangeTrigger((PhidgetVoltageInputHandle)ph, 0.0);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Set VoltageChangeTrigger: \n\t");
displayError(prc);
return;
}

prc = Phidget_getDeviceSerialNumber(ph, &serialNumber);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get DeviceSerialNumber: \n\t");
displayError(prc);
return;
}

prc = Phidget_getChannel(ph, &channel);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get Channel: \n\t");
displayError(prc);
return;
}

//Check if this is a VINT device
prc = Phidget_getHub(ph, &hub);
if (EPHIDGET_WRONGDEVICE != prc) {
prc = Phidget_getHubPort(ph, &hubPort);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get HubPort: \n\t");
displayError(prc);
return;
}
printf("\n[Attach Event]:\n\t-> Serial Number: %d\n\t-> Hub Port: %d\n\t-> Channel %d\n\n", serialNumber, hubPort, channel);
}
else {
printf("\n[Attach Event]:\n\t-> Serial Number: %d\n\t-> Channel %d\n\n", serialNumber, channel);
}

channel = 0;
}

static void CCONV onDetachHandler_pot(PhidgetHandle ph, void *ctx) {
PhidgetReturnCode prc;
PhidgetHandle hub;
int serialNumber;
int hubPort;
int channel = 0;



prc = Phidget_getDeviceSerialNumber(ph, &serialNumber);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get DeviceSerialNumber: \n\t");
displayError(prc);
return;
}

prc = Phidget_getChannel(ph, &channel);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get Channel: \n\t");
displayError(prc);
return;
}

//Check if this is a VINT device
prc = Phidget_getHub(ph, &hub);
if (EPHIDGET_WRONGDEVICE != prc) {
prc = Phidget_getHubPort(ph, &hubPort);
if (EPHIDGET_OK != prc) {
fprintf(stderr, "Runtime Error -> Get HubPort: \n\t");
displayError(prc);
return;
}
printf("\n[Detach Event]:\n\t-> Serial Number: %d\n\t-> Hub Port: %d\n\t-> Channel %d\n\n", serialNumber, hubPort, channel);
}
else {
printf("\n[Detach Event]:\n\t-> Serial Number: %d\n\t-> Channel %d\n\n", serialNumber, channel);
}
channel = 0;

}

static void CCONV onErrorHandler_pot(PhidgetHandle phid, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString) {

fprintf(stderr, "[Phidget Error Event] -> %s (%d)\n", errorString, errorCode);
}

static void CCONV onAttachHandler_motor(PhidgetHandle phid, void *ctx) {
PhidgetReturnCode res;
int hubPort;
int channel;
int serial;

res = Phidget_getDeviceSerialNumber(phid, &serial);
if (res != EPHIDGET_OK) {
fprintf(stderr, "failed to get device serial number\n");
return;
}

res = Phidget_getChannel(phid, &channel);
if (res != EPHIDGET_OK) {
fprintf(stderr, "failed to get channel number\n");
return;
}

res = Phidget_getHubPort(phid, &hubPort);
if (res != EPHIDGET_OK) {
fprintf(stderr, "failed to get hub port\n");
hubPort = -1;
}

if (hubPort == -1)
printf("channel %d on device %d attached\n", channel, serial);
else
printf("channel %d on device %d hub port %d attached\n", channel, serial, hubPort);
}

static void CCONV onDetachHandler_motor(PhidgetHandle phid, void *ctx) {
PhidgetReturnCode res;
int hubPort;
int channel;
int serial;

res = Phidget_getDeviceSerialNumber(phid, &serial);
if (res != EPHIDGET_OK) {
fprintf(stderr, "failed to get device serial number\n");
return;
}

res = Phidget_getChannel(phid, &channel);
if (res != EPHIDGET_OK) {
fprintf(stderr, "failed to get channel number\n");
return;
}

res = Phidget_getHubPort(phid, &hubPort);
if (res != EPHIDGET_OK)
hubPort = -1;

if (hubPort != -1)
printf("channel %d on device %d detached\n", channel, serial);
else
printf("channel %d on device %d hub port %d detached\n", channel, hubPort, serial);
}

static void CCONV errorHandler_motor(PhidgetHandle phid, void *ctx, Phidget_ErrorEventCode errorCode, const char *errorString) {

fprintf(stderr, "Error: %s (%d)\n", errorString, errorCode);
}

static void CCONV onPositionChangeHandler(PhidgetRCServoHandle ch, void *ctx, double position) {

//printf("[Motor Event] New Thrust set to %f\n", position);

}

static void CCONV onVoltageChangeHandler(PhidgetVoltageInputHandle pvih, void *ctx, double volt) {

//printf("[Potensiometer Event] Voltage changed to %f\n", volt);
}

void displayError(PhidgetReturnCode returnCode) {
    /*Prints error message to screen*/

    PhidgetReturnCode prc;
    const char* error;

    prc = Phidget_getErrorDescription(returnCode, &error);
    if (EPHIDGET_OK != prc) {
        fprintf(stderr, "Runtime Error -> Getting ErrorDescription: \n\t");
        displayError(prc);
        return;
    }

    fprintf(stderr, "Desc: %s\n", error);
}


int setHandlers(PhidgetVoltageInputHandle potHandle, PhidgetRCServoHandle motorHandle) {
    /* Sets event handlers for the potensiometer and the motor.
     * These handlers will be fired if the event happens.
     * CCONV is defined "Calling Conventions" from Phidget library.
     *
     * Return 0 if successful, 1 otherwise. Relevant messages are printed to screen.
     */
    PhidgetReturnCode prc;

    printf("\nSetting potensiometer handlers: OnAttach, OnDetach, OnError and OnVoltageChange\n");
    prc = Phidget_setOnAttachHandler((PhidgetHandle) potHandle, onAttachHandler_pot, NULL);
    if (EPHIDGET_OK != prc) {
        fprintf(stderr, "Runtime Error -> Set Potensiometer Attach Handler: \n\t");
        displayError(prc);
        return 1;
    }

    prc = Phidget_setOnDetachHandler((PhidgetHandle) potHandle, onDetachHandler_pot, NULL);
    if (EPHIDGET_OK != prc) {
        fprintf(stderr, "Runtime Error -> Set Potensiometer Detach Handler: \n\t");
        displayError(prc);
        return 1;
    }

    prc = Phidget_setOnErrorHandler((PhidgetHandle) potHandle, onErrorHandler_pot, NULL);
    if (EPHIDGET_OK != prc) {
        fprintf(stderr, "Runtime Error -> Set Potensiometer Error Handler: \n\t");
        displayError(prc);
        return 1;
    }

    prc = PhidgetVoltageInput_setOnVoltageChangeHandler( potHandle, onVoltageChangeHandler, NULL);
    if (EPHIDGET_OK != prc) {
        fprintf(stderr, "Runtime Error -> Set Potensiometer OnVoltageChange Handler: \n\t");
        displayError(prc);
        return 1;
    }

    printf("Setting motor handlers: OnAttach, OnDetach, OnError and OnPositionChange\n");
    prc = Phidget_setOnAttachHandler((PhidgetHandle) motorHandle, onAttachHandler_motor, NULL);
    if (prc != EPHIDGET_OK) {
        fprintf(stderr, "Runtime Error -> Set Motor Attach Handler: \n\t");
        return 1;
    }

    prc = Phidget_setOnDetachHandler((PhidgetHandle) motorHandle, onDetachHandler_motor, NULL);
    if (prc != EPHIDGET_OK) {
        fprintf(stderr, "Runtime Error -> Set Motor Detach Handler: \n\t");
        return 1;
    }

    prc = Phidget_setOnErrorHandler((PhidgetHandle) motorHandle, errorHandler_motor, NULL);
    if (prc != EPHIDGET_OK) {
        fprintf(stderr, "Runtime Error -> Set Motor Error Handler: \n\t");
        return 1;
    }

    prc = PhidgetRCServo_setOnPositionChangeHandler( motorHandle, onPositionChangeHandler, NULL);
    if (prc != EPHIDGET_OK) {
        fprintf(stderr, "Runtime Error -> Set Motor OnPositionChange Handler: \n\t");
        return 1;
    }

    printf("All event handlers set\n");
    return 0;
}
