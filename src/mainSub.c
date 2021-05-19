#include "main.h"



void newSetPoint(){
    /* Prompts a new setPoint from the user. Checks if input is valid.
     *
     * Valid -> redefine setPoint to new value.
     * Not valid -> Use  default value
     */
    printf("Enter a target position[cm] as integer between 10 and 60:  \n");
    int num;
    char term;
    if(scanf("%d%c",&num,&term)!=2|| term !='\n'||num<10 || num>60)
        printf("Invalid input. Target position set to default: %i\n", setPoint);
    else {
        #undef setPoint
        #define setPoint num
        printf("Valid Input. Target position set to %i\n", setPoint);
    }
}

double getUnixTime() {

    struct timespec tv;

    if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

    return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

void adjustValue(float *value, int max, int min, char name[16] ){
    /*  Changes value to limit set by max or min if limit breached.*/

    if (*value>max){
        *value = max;
        printf("[Maximum %s reached] --> Adjusted to %i \n", name, max);
    } else if (*value<min){
        *value = min;
        printf("[Minimum %s reached] --> Adjusted to %i \n", name, min);
    }
}