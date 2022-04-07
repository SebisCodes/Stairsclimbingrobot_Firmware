/*
*   mmain.cpp - A code to control the stairsclimbing robot using the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/

#define Debug

#include "mbed.h"
#include "Robot.h"
#include "Config.h"
#include <stdio.h>

/*
*  Define a new robot variable 
*/
Robot myRobot;
/*
*   To make the code beautiful, each process is described by an own enum
*/
enum PROCEDURES {WAIT_FOR_START, HOMING, DRIVE_FORWARD, DRIVE_BACKWARD, GO_UP, GO_DOWN, STOP};
int 

int main()
{    
    /*
    * Initialize the Robot class
    */
    myRobot = new Robot();
    while (true) {
        switch (myRobot.getProcedure()) {
            case WAIT_FOR_START:
                break;
            case HOMING:
                break;
            case HOMING:
                break;
            case HOMING:
                break;
            case HOMING:
                break;
        }
    }
}

