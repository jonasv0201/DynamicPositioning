#include "main.h"

void gnuPipe(const char *fileName){
    /*  GnuPipe plots with gnuplots by opening a terminal and entering Gnuplot commands.
     *
     *  It reads the log from fileName[dat], which has following format:
     *  time [sec], position[cm], prc-Error, input , error[cm], P-term, I-term, D-term, Offset
     *
     */


    printf("Creating Gnuplots using a Gnuplot-Pipe\n");

    //Open terminal to write gnuplot commands.
    FILE *gPipe = popen("gnuplot", "w");
    if (gPipe==NULL) {
        printf("Error opening pipe to GNU plot. Check if you have it! \n");
    }

    /* Plotting the position and error-percentage as a function of time */
    fprintf(gPipe, "set key reverse Left outside\n");
    fprintf(gPipe, "set grid x y\n");
    fprintf(gPipe, "set title 'Position and Error as Percentage' font ',20'\n");
    fprintf(gPipe, "set xlabel 'Time [sec]';set ylabel 'Position [cm]'; set y2label 'Error [%%]'\n");
    fprintf(gPipe, "set autoscale xy\n");
    fprintf(gPipe, "set y2range [-40:60]; set y2tics nomirror 20\n");
    fprintf(gPipe, "set tics out\n");
    fprintf(gPipe, "plot '%s' w l lt 1 t 'Position',", fileName);
    fprintf(gPipe, "'%s' u 1:3 w l lt 3 t 'prc-Error' axes x1y2, %d w l lt 4 t 'Set point\n", fileName, setPoint);
    fprintf(gPipe, "set term png\n");
    fprintf(gPipe, "set output '../logs/position%d.png'\n", fileNum);
    fprintf(gPipe, "replot\n");

    /* Plotting the output to motor and error as a function of time */
    fprintf(gPipe, "reset\n");
    fprintf(gPipe, "set key reverse Left outside\n");
    fprintf(gPipe, "set grid x y\n");
    fprintf(gPipe, "set title 'Input to Motor and Error' font ',20'\n");
    fprintf(gPipe, "set xlabel 'Time [sec]';set ylabel 'Thrust'\n");
    fprintf(gPipe, "set tics out\n");
    fprintf(gPipe, "set y2label 'Distance [cm]'\n");
    fprintf(gPipe, "set y2tics nomirror 20\n");
    fprintf(gPipe, "plot '%s' using 1:4 w l lt 1 t 'Input',", fileName);
    fprintf(gPipe, "'%s' using 1:5 w l lt 3 t 'Error' axes x1y2\n", fileName);
    fprintf(gPipe, "set term png\n");
    fprintf(gPipe, "set output '../logs/input%d.png' \n", fileNum);
    fprintf(gPipe, "replot\n");

    /* Plotting the output and the P, I and D contributions */
    fprintf(gPipe, "reset\n");
    fprintf(gPipe, "set key reverse Left outside\n");
    fprintf(gPipe, "set grid x y\n");
    fprintf(gPipe, "set title 'P, I and D terms' font ',20'\n");
    fprintf(gPipe, "set xlabel 'Time [sec]';set ylabel 'Thrust'\n");
    fprintf(gPipe, "set tics out\n");
    fprintf(gPipe, "plot '%s' using 1:4 w l lt 1 t 'TotalInput',", fileName);
    fprintf(gPipe, "'%s' using 1:6 w l lt 2 t 'Proportional','%s' using 1:7 w l lt 3 t 'Integral'," , fileName, fileName);
    fprintf(gPipe, "'%s' using 1:8 w l lt 4 t 'Derivative',%d w l lt 6 t 'OffSetInput\n ", fileName, offSetMotor);

    fprintf(gPipe, "set term png\n");
    fprintf(gPipe, "set output '../logs/inputTerms%d.png' \n", fileNum);
    fprintf(gPipe, "replot\n");
    fprintf(gPipe, "reset\n");

    //Close the pipe
    fclose(gPipe);

    printf("gnuPipe finished. Plots are in the logs folder.\n");
}
