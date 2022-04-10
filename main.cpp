/*
*   main.cpp - A code to control the stairsclimbing robot using the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/

#define DEBUG

#include <stdio.h>
#include "mbed.h"
#include "PM2_Libary.h"
#include "Robot.h"



        /*
        *   PINOUT CONFIG
        */
       /* const PinName PIN_SWITCH_START = PC_13;
        const PinName PIN_SWITCH_RESET = PC_12;
        const PinName PIN_SWITCH_Z_MIN = PA_0;
        const PinName PIN_SWITCH_Z_MAX = PA_1;
        const PinName PIN_SWITCH_IR_BACK = PC_7;
        const PinName PIN_SWITCH_IR_FRONT = PA_6;

        //SENSORS
        const PinName PIN_SENSOR_DISTANCE = PC_2;

        //MOTORS
        const PinName PIN_MOTOR_MAINBODY = PB_13;
        const PinName PIN_MOTOR_SIDEBODY = PA_9;
        const PinName PIN_MOTOR_ZAXIS = PA_10;
        const PinName PIN_MOTOR_ENABLE = PB_15;

        //LED
        const PinName PIN_LED_RUN = PB_2;
        const PinName PIN_LED_ERROR = PC_8;

        //BUZZER
        const PinName PIN_BUZZER = PB_14;*/

//FastPWM M_MB(PIN_MOTOR_MAINBODY);
//Define a new robot variable 
Robot *myRobot;

//To make the code beautiful, each process is described by an own enum
enum PROCEDURES {WAIT_FOR_START, HOMING, DRIVE_FORWARD, DRIVE_BACKWARD, GO_UP, GO_DOWN, STOP};

//Counter for the amount of stairs
int stairsCounter;

int main()
{    
    myRobot = new Robot();
    /*
    * Initialize the Robot class
    */
    
    /*astPWM M_SB2(PB_13);
    //this->M_SB() = M_SB2;
    M_SB2.period(0.2f);
    M_SB2.write(0.5f);
    DigitalOut en(PB_15);
    en = 1;*/
    //myRobot->driveZ(1);
    myRobot->driveSB(0);
    //myRobot->driveZ(-1);
    
    stairsCounter = 0;
    while (true) {
        switch (1) {
            case WAIT_FOR_START:
                break;
            case HOMING:
                break;
            case DRIVE_FORWARD:
                break;
            case DRIVE_BACKWARD:
                break;
            case GO_UP:
                break;
            case GO_DOWN:
                break;
            case STOP:
                break;
        }
    }
}

