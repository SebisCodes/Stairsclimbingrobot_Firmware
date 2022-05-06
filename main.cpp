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
enum PROCEDURES { /*TODO: Implement other enums if needed*/ WAIT_FOR_START, HOMING, DRIVE_FORWARD, DRIVE_BACKWARD, GO_DOWN, GO_UP, IN_ERROR, STOP};

//Counter for the amount of stairs
int stairsCounter;

//Boolean to get out of process loop
bool running = true;

//Boolean to switch between "up" and "down"
bool down = false;

//Boolean to flag another step 
bool step = false;

//Integer to use while climbing a step
int stepSequence = 0;

int main()
{    
    //Initialize robot class
    myRobot = new Robot();

    //Setup stairs counter
    stairsCounter = 0;
    
    while (running) {
        switch (myRobot->getProcedureCode()) {
            case WAIT_FOR_START:
                //TODO: Implement wait for start process
                if(myRobot->getStartSwitch()){
                    step = true;
                    stepSequence = 0;
                    myRobot->resetTaskTimer();
                    myRobot->setProcedureCode(HOMING);
                }
                break;
            case HOMING:
                //TODO: Implement homing process
                if(myRobot->getTaskMillis()>=10000){
                    myRobot->setError(true);
                    myRobot->setProcedureCode(IN_ERROR);
                }
                if(!myRobot->getMaxZSwitch()){
                    myRobot->driveZ(1);
                }else{
                    myRobot->slowMotorStop();
                    myRobot->setProcedureCode(DRIVE_FORWARD);
                }
                
                break;
            case DRIVE_FORWARD:
                //TODO: Implement forward driving process
                myRobot->driveH(1);
                if(stepSequence == 0){
                    if(myRobot->getFrontIRSwitch()){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_DOWN);
                    }
                }else if(stepSequence == 1){
                    if(myRobot->getMiddleIRSwitch()){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_UP);
                    }  
                }else if(stepSequence == 2){
                    //TODO: find good value
                    if(myRobot->getTaskMillis()>=3000){
                        stepSequence = 0;
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(STOP);
                    }
                }else { 
                    myRobot->setError(true);
                    myRobot->setProcedureCode(IN_ERROR); 
                }
                break;
            case DRIVE_BACKWARD:
                //TODO: Same as above...
                myRobot->driveH(0);
                if(stepSequence == 0){
                    if(myRobot->getBackIRSwitch()){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_UP);
                    }
                }else if(stepSequence == 1){
                    if(myRobot->getFrontIRSwitch()){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_DOWN);
                    }  
                }else if(stepSequence == 2){
                    //TODO: find good value
                    if(myRobot->getTaskMillis()>=3000){
                        stepSequence = 0;
                        myRobot->resetTaskTimer();
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(STOP);
                    }
                }else { 
                    myRobot->setError(true); 
                    myRobot->setProcedureCode(IN_ERROR);
                }
                //myRobot->setProcedureCode(...);
                break;
            case GO_DOWN:
                //TODO: ...
                myRobot->driveZ(1);
                if(!down){
                    //TODO: Find working IR sensor value.
                    if(myRobot->getIRSensorValue()<0.1 && !step){
                        step = true;
                        stepSequence = -1;  //will be set to 0
                        stairsCounter += 1;
                    }
                    if(myRobot->getMaxZSwitch()){
                        stepSequence += 1;  //0 if there is a next step, 2 if there is none
                        step = false;
                        myRobot->slowMotorStop();
                        myRobot->resetTaskTimer();
                        myRobot->setProcedureCode(DRIVE_FORWARD);
                    }
                }else{
                    if(myRobot->getMaxZSwitch()){
                        stairsCounter -= 1;
                        if(stairsCounter > 0){
                            stepSequence = 0;
                        }else{
                            myRobot->resetTaskTimer();
                            stepSequence = 2;
                        }
                        myRobot->setProcedureCode(DRIVE_BACKWARD);
                    }
                }
                //myRobot->setProcedureCode(...);
                break;
            case GO_UP:
                //TODO: ...
                myRobot->driveZ(0);
                if(myRobot->getMinZSwitch()){
                    if(!down){
                        stepSequence = 0;
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(DRIVE_FORWARD);
                    }else{
                        stepSequence += 1;
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(DRIVE_BACKWARD);
                    }
                }
                //myRobot->setProcedureCode(...);
                break;
            case IN_ERROR:
                if (myRobot->getStartSwitch()) {
                    myRobot->setError(false);
                    myRobot->setProcedureCode(HOMING);
                }
                break;
            case STOP:
                //TODO: ...
                //myRobot->setProcedureCode(...);
                if(!down){
                    if(myRobot->getStartSwitch() && myRobot->getTaskMillis()>5000){
                        down = true;
                        myRobot->setProcedureCode(DRIVE_BACKWARD);
                    }
                }else{
                    running = false; //Get out of while(running) loop
                }
                break;
            //TODO: Add more cases if needed
        }
    }
}

