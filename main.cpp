/*
*   main.cpp - A code to control the stairsclimbing robot using the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/
#include <stdio.h>
#include "mbed.h"
#include "PM2_Libary.h"
#include "Robot.h"



//Define a new robot variable 
Robot *myRobot;

//To make the code beautiful, each process is described by an own enum
enum PROCEDURES { /*TODO: Implement other enums if needed*/ WAIT_FOR_START, HOMING, DRIVE_FORWARD, DRIVE_BACKWARD, GO_UP, GO_DOWN, STOP};

//Counter for the amount of stairs
int stairsCounter;

int main()
{    
    //Initialize robot class
    myRobot = new Robot();

    //Setup stairs counter
    stairsCounter = 0;
    
    myRobot->driveSB(0);
    
    while (true) {
        switch (myRobot->getProcedureCode()) {
            case WAIT_FOR_START:
                //TODO: Implement wait for start process
                myRobot->setProcedureCode(HOMING);
                break;
            case HOMING:
                //TODO: Implement homing process
                myRobot->setProcedureCode(DRIVE_FORWARD);
                break;
            case DRIVE_FORWARD:
                //TODO: Implement forward driving process
                //myRobot->setProcedureCode(...);
                break;
            case DRIVE_BACKWARD:
                //TODO: Same as above...
                //myRobot->setProcedureCode(...);
                break;
            case GO_UP:
                //TODO: ...
                //myRobot->setProcedureCode(...);
                break;
            case GO_DOWN:
                //TODO: ...
                //myRobot->setProcedureCode(...);
                break;
            case STOP:
                //TODO: ...
                //myRobot->setProcedureCode(...);
                break;
            //TODO: Add more cases if needed
        }
    }
}

