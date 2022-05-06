/*
*   main.cpp - A code to control the stairsclimbing robot using the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/
#include <cstdio>
#include <stdio.h>
#include "mbed.h"
#include "PM2_Libary.h"
#include "Robot.h"



//Define a new robot variable 
Robot *myRobot;

//To make the code beautiful, each process is described by an own enum
enum PROCEDURES { /*TODO: Implement other enums if needed*/ WAIT_FOR_START, INIT, SETUP, WAIT_FOR_DOWN, FW_TO_STEP, FW_ON_STEP, FW_LAST_STEP, BW_TO_EDGE, BW_OVER_EDGE, BW_LAST_STEP, GO_DOWN, FW_GO_UP, BW_GO_UP, IN_ERROR, STOP};

//Counter for the amount of stairs
int stairsCounter;

//Boolean to get out of process loop
bool running = true;
bool run = false;

//Boolean to switch between "up" and "down"
bool down = false;

//Boolean to flag another step 
bool step = false;

void eStop(){
    myRobot->setError(true);
    myRobot->setWarning(true);
    myRobot->emergencyStop();
}

int main()
{    
    //Initialize robot class
    myRobot = new Robot();
    myRobot->SW_START->fall(eStop);
    //Setup stairs counter

    while (running) {
        switch (myRobot->getProcedureCode()) {
            case WAIT_FOR_START:
                //TODO: Implement wait for start process
                printf("WAIT_FOR_START\n");
                while(!myRobot->getStartSwitch()); // wait
                myRobot->setProcedureCode(INIT);
                break;
            
            case INIT:            
                printf("INIT\n");
                step = true;
                down = false;
                stairsCounter = 0;
                if(myRobot->getInitPos()){
                    myRobot->setProcedureCode(FW_TO_STEP);
                }else {
                    myRobot->setProcedureCode(SETUP);
                }
                break;

            case SETUP:
                printf("SETUP\n");
                if (!myRobot->getMaxZSwitch()) {
                    myRobot->driveZ(1);
                    myRobot->resetTaskTimer();
                    run = true;
                }
                while (run) {
                    if(myRobot->getTaskMillis()>=10000){
                        myRobot->setError(true);
                        myRobot->setProcedureCode(IN_ERROR);
                        run = false;
                    }
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                if(myRobot->getInitPos()){
                    myRobot->setProcedureCode(FW_TO_STEP);
                }else {
                    myRobot->setError(true);
                    myRobot->setProcedureCode(IN_ERROR);
                }
            break;

            case FW_TO_STEP:
                printf("FW_TO_STEP\n");
                if(!myRobot->getFrontIRSwitch()){
                    myRobot->driveH(1);
                    run = true;    
                }
                while(run) {
                    if(myRobot->getFrontIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                myRobot->setProcedureCode(GO_DOWN);
                break;
            
            case FW_ON_STEP:
                printf("FW_ON_STEP\n");
                if(!myRobot->getMiddleIRSwitch()){
                    myRobot->driveH(1);
                    run = true;
                }
                while(run){
                    if(myRobot->getMiddleIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    } 
                }
                myRobot->setProcedureCode(FW_GO_UP);
                break;

            case FW_LAST_STEP:
                printf("FW_LAST_STEP\n");
                myRobot->resetTaskTimer();
                myRobot->driveH(1);
                run = true;
                while(run){
                    if(myRobot->getTaskMillis()>=3000){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                myRobot->setProcedureCode(WAIT_FOR_DOWN);
                break;
    
            case WAIT_FOR_DOWN:
                printf("WAIT_FOR_DOWN\n");
                while(!myRobot->getStartSwitch()); // wait 
                down = true;
                myRobot->setProcedureCode(BW_TO_EDGE);
                break;

            case BW_TO_EDGE:
                printf("BACKWARD_TO EDGE\n");
                if(myRobot->getBackIRSwitch()){
                    myRobot->driveH(0);
                    run = true;
                }
                while(true){
                    if(!myRobot->getBackIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                myRobot->setProcedureCode(BW_GO_UP);
                break;

            case BW_OVER_EDGE:
                printf("BW_OVER_EDGE\n");
                if(myRobot->getFrontIRSwitch()){
                    myRobot->driveH(0);
                    run = true;
                }
                while(true){
                    if(!myRobot->getFrontIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                myRobot->setProcedureCode(GO_DOWN);
                break;

            case BW_LAST_STEP:
                printf("BW_LAST_STEP\n");
                myRobot->resetTaskTimer();
                run = true;
                while(true){
                    if(myRobot->getTaskMillis()>=3000){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                myRobot->setProcedureCode(STOP);
                break;

            case GO_DOWN:
                printf("GO_DOWN\n");
                if(!myRobot->getMinZSwitch()){
                    myRobot->driveZ(0);
                    run = true;
                }
                while(run){
                    if(myRobot->getMinZSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                if(!down){
                    myRobot->setProcedureCode(FW_ON_STEP);
                }else{
                    myRobot->setProcedureCode(BW_OVER_EDGE);
                }
                break;

            case FW_GO_UP:
                printf("FW_GO_UP\n");
                if(!myRobot->getMaxZSwitch()){
                    myRobot->driveZ(1);
                    run = true;
                }
                while(run){
                    if(myRobot->getIRSensorValue()<0.1 && !step){
                        step = true;
                        stairsCounter += 1;
                    }
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        if(step){
                            myRobot->setProcedureCode(FW_TO_STEP);
                        }else{
                            myRobot->setProcedureCode(FW_LAST_STEP);
                        }
                        step = false;
                        run = false;
                    }
                }
            break;

            case BW_GO_UP:
                printf("BW_GO_UP\n");
                if(!myRobot->getMaxZSwitch()){
                    myRobot->driveZ(1);
                    run = true;
                }
                while(run){
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        stairsCounter -= 1;
                        if(stairsCounter > 0){
                            myRobot->setProcedureCode(BW_TO_EDGE);
                        }else{
                            myRobot->setProcedureCode(BW_LAST_STEP);
                        }

                    }
                }
            break;
            
            case IN_ERROR:
                printf("IN_ERROR\n");
                while(!myRobot->getStartSwitch()); //wait 
                myRobot->setError(false);
                myRobot->setProcedureCode(INIT);
                break;

            case STOP:
                printf("STOP\n");
                running = false; //Get out of while(running) loop
                myRobot->setProcedureCode(WAIT_FOR_START);
                break;
            //TODO: Add more cases if needed
        }
    }
}