#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pigpio.h>

/*
File   : trigger_fan.c
Author : Zachary Milke
Description :
    A simple script that sends a trigger signal to a Pi Pico, indicating the
    desired state of the cooling fan that blows over the Pi. A heartbeat signal
    is first sent to the Pico and if a response is received, the program
    continues, otherwise, it exits.

Created on : 13-APR-2023
Updated on : 17-APR-2023

Args
-----
argv[1] : float [required]
    Approximate cpu temperature. If cpuTEMP>=TRIGGER_TEMP, set fan state HIGH
    otherwise, set fan state LOW.

Compilation
-----
gcc -Wall -pthread -o trigger_fan trigger_fan.c -lpigpio -lrt
*/

#define TRIGGER_OUT 4   // GPIO pin that sends fan state signal to Pico.
#define TRIGGER_TEMP 40 // Temperature in C that fan is triggered at.

#define HB_OUT 17     // GPIO pin that sends hearbeat signal to Pico.
#define HB_IN 27      // GPIO pin that listens for heartbeat response from Pico.
#define HB_DELAY 0.1f // Delay in seconds after setting HB_OUT to HIGH before checking response.

bool heartbeat(); // Method that sends/listens for heartbeat signal.

int main(int argc, char *argv[])
{
    float cpuTemp; // Temperature of CPU in degress celsius.
    int fanState; // Indicates whether the fan will be set to ON/OFF.
    bool reachedTarget; // Indicates whether the target temperature has been reached yet.

    // Verify proper amount of arguments were supplied.
    if (argc == 1)
    {
        // Not enough arguments supplied. Exit.
        fprintf(stdout, "Not enough arguments supplied!\n");
        return 1;
    }
    else if (argc == 2)
    {
        // Convert CPU temperature argument to float.
        cpuTemp = atof(argv[1]);
        fprintf(stdout, "CPU Temperature = %.2f. ", cpuTemp);
    
        // Determine whether fan state should be HIGH/LOW.
        if (cpuTemp >= TRIGGER_TEMP)
        {
            // Temperature is above trigger, set HIGH.
            fanState = 1;
        }
        else
        {
            // Temperature is below trigger, set LOW.
            fanState = 0;
        }

        fprintf(stdout, "Fan state = %d\n", fanState);
    }

    // Inititialize the GPIO library.
    if (gpioInitialise() < 0)
    {
        // If not successful, return error.
        return 1;
    }

    // Setup the TRIGGER_OUT, HB_OUT as output pins. Then, then HB_IN as an input pin.
    gpioSetMode(TRIGGER_OUT, PI_OUTPUT);
    gpioSetMode(HB_OUT, PI_OUTPUT);
    gpioSetMode(HB_IN, PI_INPUT);

    // Send and check for a heartbeat signal.
    if (heartbeat())
    {
        // Heartbeat received, write the output signal to the GPIO pin.
        gpioWrite(TRIGGER_OUT, fanState);
    }
    else
    {
        // No hearbeat received, exit without writing signal.
        fprintf(stdout, "Heartbeat signal not received. Exiting.\n");
    }

    // Terminate and cleanup the GPIO library, then exit.
    gpioTerminate();
    return 0;
}

bool heartbeat()
{
    // Boolean representing acknowledgement from Pico device that hearbeat
    // signal was recieved.
    bool hb_ack = false;

    // Set the heartbeat signal to HIGH.
    gpioWrite(HB_OUT, 1);

    // Wait a small delay, then listen for a response.
    sleep(HB_DELAY);
    if (gpioRead(HB_IN) == 1)
    {
        // Heartbeat acknowledge received from Pico.
        hb_ack = true;
    }

    // Reset the HB_OUT pin to LOW, and return hb_ack.
    gpioWrite(HB_OUT, 0);
    return hb_ack;
}
