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

//Boolean to get out of process loop
bool running = true;

//Boolean to switch between "up" and "down"
bool down = false;

//Boolean to flag another step 
bool step = false;

//Integer to use while climbing a step
int sequence = 0;

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
                    myRobot->setProcedureCode(HOMING);
                }
                break;
            case HOMING:
                //TODO: Implement homing process
                if(myRobot->getMinZSwitch()){
                    myRobot->driveZ(0);
                }else{
                    myRobot->slowMotorStop();
                    myRobot->setProcedureCode(DRIVE_FORWARD);
                }
                
                break;
            case DRIVE_FORWARD:
                //TODO: Implement forward driving process
                myRobot->driveMB(1);
                if(sequence == 0){
                    if(myRobot->getFrontIRSwitch() && !down){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_UP);
                    }
                }else if(sequence == 1){
                    if(myRobot->getMiddleIRSwitch() && !down){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_DOWN);
                    }        
                }else if(sequence ==2){
                    if(myRobot->getMiddleIRSwitch() && !down){
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(GO_UP);
                    }
                }
                break;
            case DRIVE_BACKWARD:
                //TODO: Same as above...
                
                //myRobot->setProcedureCode(...);
                break;
            case GO_UP:
                //TODO: ...
                myRobot->driveZ(1);
                if(!down){
                    if(myRobot->getIRSensorValue()<=1 && !step){
                        step = true;
                        stairsCounter +=1;
                    }
                    if(myRobot->getMaxZSwitch()){
                        sequence +=1;
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(DRIVE_FORWARD);
                    }
                }
                //myRobot->setProcedureCode(...);
                break;
            case GO_DOWN:
                //TODO: ...
                myRobot->driveZ(0);
                if(!down){
                    if(myRobot->getMinZSwitch()){
                        sequence += 1;
                        myRobot->slowMotorStop();
                        myRobot->setProcedureCode(DRIVE_FORWARD);
                    }
                }
                //myRobot->setProcedureCode(...);
                break;
            case STOP:
                //TODO: ...
                //myRobot->setProcedureCode(...);
                running = false; //Get out of while(running) loop
                break;
            //TODO: Add more cases if needed
        }
    }
}

