/*
*   main.cpp - A code to control the stairsclimbing robot using the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/
#include <stdio.h>
#include "mbed.h"
#include "PM2_Libary.h"
#include "Robot.h"

#define TIMEOUT 60000

//Define a new robot variable 
Robot *myRobot;

//To make the code beautiful, each process is described by an own enum
enum PROCEDURES {
    HOMING, //0
    WAIT_FOR_START, //1
    DRIVE_FORWARD_AT_BEGINNING, //2
    DRIVE_Z_DOWN, //3
    DRIVE_FORWARD_ON_STAIR, //4
    DRIVE_Z_UP, //5
    DRIVE_FORWARD_AT_LAST_STEP, //6
    WAIT_FOR_SIGNAL, //7
    DRIVE_BACK_AT_FIRST_STEP, //8
    DRIVE_BACKWARD_ON_STAIR, //9
    DRIVE_BACKWARD_AT_LAST_STEP, //10
    END //11
    };

//Counter for the amount of stairs
int stepCounter;

//Boolean to get out of process loop
bool run = true; //Main loop running
bool waiting = false; //Disable eStop while waiting

//Boolean to switch between "up" and "down"
bool down = false;

//Boolean to flag another step 
bool step = false;

//Integer to use while climbing a step
int stepSequence = 0;

void eStop(bool instand) {
    if (!waiting) {
        //make sure the button isn't clicked after several time
        long long t = myRobot->getMillis();
        while (myRobot->getMillis() < t+50 && !instand);
        if (!myRobot->getStartSwitch()) {
            myRobot->setError(true);
            myRobot->emergencyStop();
        }
    }
}
void switchStop() {
    eStop(false);
}

int main()
{    
    //Initialize robot class
    myRobot = new Robot();
    myRobot->SW_START->rise(&switchStop);
    myRobot->setProcedureCode(HOMING);
    //Setup stairs counter
    stepCounter = 0;

    //Run main loop
    while (run) {
        printf("%d\n",myRobot->getProcedureCode());
        switch (myRobot->getProcedureCode()) {
            case HOMING:
                #define CONDITION myRobot->getMaxZSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveZ(1,false);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    myRobot->setProcedureCode(WAIT_FOR_START);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                //Go to next procedure
                myRobot->setProcedureCode(WAIT_FOR_START);
                #undef CONDITION
                break;
            case WAIT_FOR_START:
                while(!myRobot->getStartSwitch());
                myRobot->setWarning(false);
                myRobot->setProcedureCode(DRIVE_FORWARD_AT_BEGINNING);
                break;
            case DRIVE_FORWARD_AT_BEGINNING:
                #define CONDITION myRobot->getFrontIRSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveH(1, true);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                //Go to next procedure
                myRobot->setProcedureCode(DRIVE_Z_DOWN);
                #undef CONDITION
                break;
            case DRIVE_Z_DOWN:
                #define CONDITION myRobot->getMinZSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveZ(0,false);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                //Go to next procedure
                if (down) {
                    myRobot->setProcedureCode(DRIVE_BACKWARD_ON_STAIR);
                } else {
                    myRobot->setProcedureCode(DRIVE_FORWARD_ON_STAIR);
                }
                #undef CONDITION
                break;
            case DRIVE_FORWARD_ON_STAIR:
                #define CONDITION myRobot->getMiddleIRSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveH(1, true);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                //Go to next procedure
                myRobot->setProcedureCode(DRIVE_Z_UP);
                #undef CONDITION
                break;
            case DRIVE_Z_UP:
                #define CONDITION myRobot->getMaxZSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveZ(1,false);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                step = false;
                
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                    if (myRobot->getIRSensorValue() > 0.3) {
                        if (!step) myRobot->setWarning(true);
                        step = true;
                    }
                }
                if (down) {
                    if (stepCounter < 0) {
                        myRobot->setProcedureCode(DRIVE_BACKWARD_AT_LAST_STEP);
                    } else {
                        stepCounter--;
                        myRobot->setProcedureCode(DRIVE_BACK_AT_FIRST_STEP);
                    }
                } else {
                    if (step) {
                        stepCounter++;
                        myRobot->setProcedureCode(WAIT_FOR_START);
                    } else {
                        myRobot->setProcedureCode(DRIVE_FORWARD_AT_LAST_STEP);
                    }

                }
                #undef CONDITION
                break;
            case DRIVE_FORWARD_AT_LAST_STEP:
                //Drive forward for a defined time
                myRobot->driveH(1, true);
                myRobot->resetTaskTimer();
                while(myRobot->getTaskMillis() < 2000);
                myRobot->setProcedureCode(WAIT_FOR_SIGNAL);
                break;
            case WAIT_FOR_SIGNAL:
                waiting = true;
                //Blink to say "grab your drink"
                myRobot->setWarning(true);
                while(waiting) {
                    myRobot->resetTaskTimer();
                    myRobot->setWarning(true);
                    while(myRobot->getTaskMillis() < 375 && waiting) waiting = (myRobot->getIRSensorValue() < 0.5f);
                    myRobot->resetTaskTimer();
                    myRobot->setWarning(false);
                    while(myRobot->getTaskMillis() < 375 && waiting) waiting = (myRobot->getIRSensorValue() < 0.5f);
                }
                myRobot->setWarning(false);
                down = true;
                waiting = false;
                myRobot->setProcedureCode(DRIVE_BACK_AT_FIRST_STEP);
                break;
            case DRIVE_BACK_AT_FIRST_STEP:
                #define CONDITION !myRobot->getBackIRSwitch()

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveH(0, true);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                myRobot->slowMotorStop();
                //Go to next procedure
                myRobot->setProcedureCode(DRIVE_Z_DOWN);
                #undef CONDITION
                break;
            case DRIVE_BACKWARD_ON_STAIR:
                #define CONDITION !myRobot->getFrontIRSwitch()
                myRobot->setUltraslowSpeed(true);

                //Check beginning conditions
                if (!CONDITION) {
                    myRobot->driveH(0, true);
                } else {
                    //Go to next procedure or set error if condition is wrong
                    eStop(true);
                    break;
                }
                //Go into loop until condition is met or timer is running out of time
                myRobot->resetTaskTimer();
                while (!CONDITION) {
                    if (myRobot->getTaskMillis() >= TIMEOUT) eStop(true); //Go to error if the timeout is over
                }
                myRobot->setUltraslowSpeed(false);
                if (stepCounter <= 0) {
                    //Go to next procedure
                    myRobot->setProcedureCode(DRIVE_Z_UP);
                } else {
                    //Go to next procedure
                    stepCounter--;
                    myRobot->setProcedureCode(DRIVE_Z_UP);
                }
                #undef CONDITION
                break;
            case DRIVE_BACKWARD_AT_LAST_STEP:
                //Drive forward for a defined time
                myRobot->driveH(0, true);
                myRobot->resetTaskTimer();
                while(myRobot->getTaskMillis() < 2000);
                myRobot->setProcedureCode(END);
                break;
            case END:
                run = false;
                break;
        }
        myRobot->slowMotorStop();
    }
}

