#include "mbed.h"

#ifndef MYCONFIG_H
#define MYCONFIG_H


        /*
        *   PINOUT CONFIG
        */
        //SWITCHES
        #define PIN_SWITCH_START PC_13
        //#define PIN_SWITCH_RESET PC_12
        #define PIN_SWITCH_Z_MIN PA_0
        #define PIN_SWITCH_Z_MAX PA_1
        #define PIN_SWITCH_IR_BACK PB_6
        #define PIN_SWITCH_IR_MIDDLE PC_7
        #define PIN_SWITCH_IR_FRONT PA_6
        #define PIN_SWITCH_EMERGENCY PC_15

        //SENSORS
        #define PIN_SENSOR_DISTANCE PC_2

        //MOTORS
        #define PIN_MOTOR_MAINBODY PB_13
        #define PIN_MOTOR_SIDEBODY PA_9
        #define PIN_MOTOR_ZAXIS PA_10
        #define PIN_MOTOR_ENABLE PB_15

        //LED
        #define PIN_LED_RUN PB_2
        #define PIN_LED_WARNING PC_8
        #define PIN_LED_ERROR PC_6

        //BUZZER
        #define PIN_BUZZER PB_14

        /*
        *   MOTOR CONTROL CONFIG
        */
        // Motor direction control: 0 = normal direction, 1 = inverted direction
        #define MOTOR_DIRECTION_MAINBODY 0
        #define MOTOR_DIRECTION_SIDEBODY 0
        #define MOTOR_DIRECTION_ZAXIS 0
        // Motor pwm: range between 0.0f and 0.5f
        #define MOTOR_PWM_MAINBODY 0.2
        #define MOTOR_PWM_MAINBODY_SLOW 0.3
        #define MOTOR_PWM_SIDEBODY 0.4
        #define MOTOR_PWM_SIDEBODY_SLOW 0.5
        #define MOTOR_PWM_ZAXIS 0.3
        #define MOTOR_PWM_ZAXIS_SLOW 0.15

        #define MOTOR_SLOW_STOP_DURATION 200

    #endif
