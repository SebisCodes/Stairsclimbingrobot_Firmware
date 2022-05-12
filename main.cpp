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
enum PROCEDURES {WAIT_FOR_START, INIT, SETUP, WAIT_FOR_DOWN, FW_TO_STEP, FW_ON_STEP, FW_LAST_STEP, BW_TO_EDGE, BW_OVER_EDGE, BW_TIMED, BW_SLOW, BW_LAST_STEP, GO_DOWN, FW_GO_UP, BW_GO_UP, IN_ERROR, STOP};

//Counter for the amount of stairs
int stairsCounter;

//Boolean to get out of process loop
bool running = true;
bool run = false;

//Boolean to switch between "up" and "down"
bool down = false;
bool peak = false;

//Boolean to flag another step 
bool step = false;

void eStop(){
    if(!peak){
        myRobot->setError(true);
        myRobot->setWarning(true);
        myRobot->emergencyStop();
    }
    
}

int main()
{    
    //Initialize robot class
    myRobot = new Robot();
    myRobot->setProcedureCode(WAIT_FOR_START);
    myRobot->SW_START->fall(eStop);

    while (running) {
        switch (myRobot->getProcedureCode()) {
            case WAIT_FOR_START:
                printf("wait for start\n"); //for troubleshooting
                //wait until the start button is pressed, then switch to init
                while(!myRobot->getStartSwitch());
                myRobot->setProcedureCode(INIT);
                break;
            
            case INIT:            
                printf("init\n"); //for troubleshooting
                //initialize variables for sequence
                down = false;
                step = false;
                run = false;
                stairsCounter = 0;
                //switch to driving forward if the initial position is reached
                //switch to setup otherwise
                if(myRobot->getInitPos()){
                    myRobot->setProcedureCode(FW_TO_STEP);
                }else {
                    myRobot->setProcedureCode(SETUP);
                }
                break;

            case SETUP:
                printf("setup\n"); //for troubleshooting
                //move side-body up, if the upper switch is not active
                if (!myRobot->getMaxZSwitch()) {
                    myRobot->driveZ(1, false);
                    myRobot->resetTaskTimer();
                    run = true;
                }
                while (run) {
                    //switch to error state, if the upper switch is not reached within a set amount of time
                    if(myRobot->getTaskMillis()>=10000){
                        myRobot->setError(true);
                        myRobot->setProcedureCode(IN_ERROR);
                        run = false;
                    }
                    //stop motor, if the upper Z-switch turns active
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                //switch to driving forward, if the initial position is reached
                //switch to error state, if the initial position is still not reached (possible issue with sensors/switches)
                if(myRobot->getInitPos()){
                    myRobot->setProcedureCode(FW_TO_STEP);
                }else {
                    myRobot->setError(true);
                    myRobot->setProcedureCode(IN_ERROR);
                }
            break;

            case FW_TO_STEP:
                printf("forward to step\n"); //for troubleshooting
                //drive forward, as long as the front IR-switch does not recognize a step
                if(!myRobot->getFrontIRSwitch()){
                    myRobot->driveH(1, false);
                    run = true;    
                }
                while(run) {
                    if(myRobot->getFrontIRSwitch()){
                        myRobot->motorStop();
                        run = false;
                    }
                }
                //switch to moving the side-body down
                myRobot->setProcedureCode(GO_DOWN);
                break;
            
            case FW_ON_STEP:
                printf("forward on step\n"); //for troubleshooting
                //drive forward, as long as the middle IR-switch does not recognize a step
                if(!myRobot->getMiddleIRSwitch()){
                    myRobot->driveH(1, false);
                    run = true;
                }
                while(run){
                    if(myRobot->getMiddleIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    } 
                }
                //switch to moving the side-body up
                myRobot->setProcedureCode(FW_GO_UP);
                break;

            case FW_LAST_STEP:
                printf("forward last step\n"); //for troubleshooting
                //drive forward for a set amount of time
                myRobot->resetTaskTimer();
                myRobot->driveH(1, false);
                run = true;
                while(run){
                    if(myRobot->getTaskMillis()>=1000){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                //switch to waiting
                myRobot->setProcedureCode(WAIT_FOR_DOWN);
                break;
    
            case WAIT_FOR_DOWN:
                printf("wait for down\n"); //for troubleshooting
                //waits on last step until beer is lifted from the robot, or at least 5 seconds
                peak = true;
                while(myRobot->getStartSwitch());
                myRobot->resetTaskTimer();
                while (!myRobot->getStartSwitch() || myRobot->getTaskMillis() < 50);
                myRobot->resetTaskTimer();
                while (myRobot->getMillis() < 2000);
                peak = false;
                myRobot->setProcedureCode(BW_TO_EDGE);
                break;

            case BW_TO_EDGE:
                printf("backward to edge\n");
                //drive backward, until the back IR-switch is no longer active (sensor beyond edge)
                if(myRobot->getBackIRSwitch()){
                    myRobot->driveH(0, false);
                    run = true;
                }
                while(run){
                    if(!myRobot->getBackIRSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                //switch to moving the side-body down
                myRobot->setProcedureCode(GO_DOWN);
                break;

            case BW_OVER_EDGE:
                printf("backward over edge\n"); //for troubleshooting
                //drive backward, until the middle IR-switch is no longer active (sensor beyond edge)
                if(myRobot->getMiddleIRSwitch()){
                    myRobot->driveH(0, true);
                    run = true;
                }
                while(run){
                    if(!myRobot->getMiddleIRSwitch()){
                        myRobot->slowMotorStop(); //remove this if next step is BW_TIMED
                        run = false;
                    }
                }
                //switch to driving slowly
                myRobot->setProcedureCode(BW_SLOW);
                break;
            case BW_SLOW:
                printf("backward slow\n"); //for troubleshooting
                //drive backward slowly, until the front IR-switch is no longer active
                if (myRobot->getFrontIRSwitch()) {
                    myRobot->driveSB(0, true);
                    run = true;
                }
                while (run) {
                    if (!myRobot->getFrontIRSwitch()) {
                        myRobot->motorStop();
                        run = false;
                    }
                }
                //switch to moving the side-body up
                myRobot->setProcedureCode(BW_GO_UP);
                break;

            case BW_TIMED:  //will be used, if driving slowly over the edge does not work
                            //motor needs to still be running from last step
                printf("backward timed\n"); //for troubleshooting
                //stop motor, after a set amount of time has passed
                myRobot->resetTaskTimer();
                run = true;
                while(run){
                    if (myRobot->getTaskMillis()>1000) { //value needs adjusting!
                        myRobot->motorStop();
                        run = false;
                    }
                }
                //switch to moving the side-body up
                myRobot->setProcedureCode(BW_GO_UP);
                break;

            case BW_LAST_STEP:
                printf("backward last step\n"); //for troubleshooting
                //drive backward, until a set amount of time has passed
                myRobot->resetTaskTimer();
                myRobot->driveH(0, false);
                run = true;
                while(run){
                    if(myRobot->getTaskMillis()>=1000){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                //switch to stop
                myRobot->setProcedureCode(STOP);
                break;

            case GO_DOWN:
                printf("go down\n"); //for troubleshooting
                //drive side-body down, as long as the lower switch is inactive
                if(!myRobot->getMinZSwitch()){
                    myRobot->driveZ(0, false);
                    run = true;
                }
                while(run){
                    if(myRobot->getMinZSwitch()){
                        myRobot->slowMotorStop();
                        run = false;
                    }
                }
                //switch to driving forward, if the robot is in the process of climbing (down = false)
                //switch to driving backward, if the robot is in the process of descending (down = true)
                if(!down){
                    myRobot->setProcedureCode(FW_ON_STEP);
                }else{
                    myRobot->setProcedureCode(BW_OVER_EDGE);
                }
                break;

            case FW_GO_UP:
                printf("forward go up\n"); //for troubleshooting
                //move side-body up, as long as the upper switch is inactive
                if(!myRobot->getMaxZSwitch()){
                    myRobot->driveZ(1, false);
                    run = true;
                }
                while(run){
                    //while moving upward, the IR-sensor value is compared with a set value
                    if(myRobot->getIRSensorValue()>0.4 && !step){
                        step = true; //flag that there is another step coming
                        stairsCounter += 1;
                    }
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        //switch to driving forward
                        if(step){
                            myRobot->setProcedureCode(FW_TO_STEP); //in case there is another step
                        }else{
                            myRobot->setProcedureCode(FW_LAST_STEP); //in case there are no more steps
                        }
                        step = false; //reset flag
                        run = false;
                    }
                }
            break;

            case BW_GO_UP:
                printf("backward go up\n"); //for troubleshooting
                //move sidebody up, as long as the upper switch is inactive
                if(!myRobot->getMaxZSwitch()){
                    myRobot->driveZ(1, false);
                    run = true;
                }
                while(run){
                    if(myRobot->getMaxZSwitch()){
                        myRobot->slowMotorStop();
                        stairsCounter -= 1; //remove one step from remaining steps
                        //switch to driving backward
                        if(stairsCounter >= 0){
                            myRobot->setProcedureCode(BW_TO_EDGE); //in case there are remaining steps
                        }else{
                            myRobot->setProcedureCode(BW_LAST_STEP); //in case there are no more steps
                        }
                        run = false;
                    }
                }
            break;
            
            case IN_ERROR:
                printf("in error\n"); //for troubleshooting
                //wait until the start switch is inactive (in case there is a beer on top of the robot)
                if(!myRobot->getStartSwitch()){
                    run = true;
                }
                while(run){
                    //reset error and switch to init if the start switch is active
                    if(myRobot->getStartSwitch()){
                        myRobot->setError(false);
                        run = false;
                        myRobot->setProcedureCode(INIT);
                    }    
                }
                break;

            case STOP:
                printf("stop\n"); //for troubleshoting
                running = false; //Get out of while(running) loop
                myRobot->setProcedureCode(WAIT_FOR_START);
                break;
        }
    }
}