/*
*   Config.h - A library to setup the configurations for the Robot.h library. 
*   Created by Sebastian G., Winterthur, April 07, 2022 
*/

#ifndef CONFIG_H
#define CONFIG_H


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

#define CONVERT_MOTOR_MAINBODY 0
#define CONVERT_MOTOR_SIDEBODY 0
#define CONVERT_MOTOR_ZAXIS 0

#endif