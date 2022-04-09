#include "mbed.h"

#ifndef MYCONFIG_H
#define MYCONFIG_H


        /*
        *   PINOUT CONFIG
        */
        //SWITCHES
        #define PIN_SWITCH_START PC_13
        #define PIN_SWITCH_RESET PC_12
        #define PIN_SWITCH_Z_MIN PA_0
        #define PIN_SWITCH_Z_MAX PA_1
        #define PIN_SWITCH_IR_BACK PC_7
        #define PIN_SWITCH_IR_FRONT PA_6

        //SENSORS
        #define PIN_SENSOR_DISTANCE PC_2

        //MOTORS
        #define PIN_MOTOR_MAINBODY PB_13
        #define PIN_MOTOR_SIDEBODY PA_9
        #define PIN_MOTOR_ZAXIS PA_10
        #define PIN_MOTOR_ENABLE PB_15

        //LED
        #define PIN_LED_RUN PB_2
        #define PIN_LED_ERROR PC_8

        //BUZZER
        #define PIN_BUZZER PB_14

        /*
        *   MOTOR CONTROL CONFIG
        */
        // Motor direction control: 0 = Normal direction, 1 = invertedDirection
        #define MOTOR_MAINBODY_DIRECTION 0
        #define MOTOR_SIDEBODY_DIRECTION 0
        #define MOTOR_ZAXIS_DIRECTION 0

        //Slow stop duration in milliseconds
        #define MOTOR_SLOW_STOP_DURATION 5000

        #endif