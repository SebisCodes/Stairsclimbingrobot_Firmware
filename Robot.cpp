/*
*   Robot.cpp - A library to control the stairsclimbing robot. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/
#include "Robot.h"



Robot::Robot() {
    /*
    *   Setup motors
    */
    this->M_MB = new FastPWM(PIN_MOTOR_MAINBODY);
    this->M_SB = new FastPWM(PIN_MOTOR_SIDEBODY);
    this->M_Z  = new FastPWM(PIN_MOTOR_ZAXIS);
    this->M_EN = new DigitalOut(PIN_MOTOR_ENABLE);
    this->motorStop();

    /*
    *   Setup error management
    */
    this->LED_error = new DigitalOut(PIN_LED_ERROR);
    this->LED_running = new DigitalOut(PIN_LED_RUN);
    this->setError(false);


} 

/**
*   Get the actual time in milliseconds since the last time reset
*   @param return int - get the actual milliseconds since the last timer reset
*/
long Robot::getMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->taskTimer.elapsed_time()).count();
}

/**
*   Get the actual time of errorTimer in milliseconds since the last time reset
*   @param return int - get the actual milliseconds of errorTimer since the last timer reset
*/
long Robot::getErrorMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->errorTimer.elapsed_time()).count();
}

/**
*   Reset the tasktimer
*/
void Robot::resetTimer() {
    this->taskTimer.reset();
}

/**
*   Store a running procedure code
*   @param procedureCode int - the procedureCode to set
*/
void Robot::setProcedureCode(int procedureCode) {
    this->procedureCode = procedureCode;
}

/**
*   Get a running procedure code
*   @param return int - the setted procedureCode
*/
int Robot::getProcedureCode() {
    return this->procedureCode;
}

/**
*   Set the enabled state of the motors
*   @param en Bool - Set the enabled state of the motors: true -> enabled, false -> disabled
*/
void Robot::enableMotors(bool en) {
    if (en) 
        *(this->M_EN) = 1;
    else
        *(this->M_EN) = 0;
}   

/**
*   Stop the motors in a fast way
*/
void Robot::motorStop() {
    //Disable motors and reset speed of all motors
    this->enableMotors(false);
    this->M_MB->write(0.5f);
    this->M_SB->write(0.5f);
    this->M_Z->write(0.5f);
}

/**
*   Stop the motors in a slow way (without a jerk)
*/
void Robot::slowMotorStop() {
    //Set the amount of steps to make a slow stop
    int steps = 10;
    //Time to stop in seconds
    int timeToStop = 3;

    //Store the speeds at beginning of the slow down process
    double startSpeeds[] = {
        this->M_MB->read(),
        this->M_SB->read(),
        this->M_Z->read()
    };
    // Reset the timer
    this->taskTimer.reset();
    //Define the actual time
    int actualTime = 0;
    //loop whilee slowing down
    while ((actualTime = this->getMillis()) < MOTOR_SLOW_STOP_DURATION) {
        M_MB->write((0.5f-startSpeeds[0])/MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[0]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
        M_SB->write((0.5f-startSpeeds[1])/MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[1]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
        M_Z->write((0.5f-startSpeeds[2])/MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[2]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
    }
    //Disable motors and reset speed of all motors
    this->motorStop();
}

/**
*   Set an error state and show it over an LED
*   @param errorState bool - If error, set true, if not set false
*/
void Robot::setError(bool errorState) {
    this->error = errorState;
    *(this->LED_error) = (error ? 1 : 0);
    *(this->LED_running) = (error ? 0 : 1);
    if (errorState) {
        motorStop();
    }
}

/**
*   Ge the actual error state
*   @param return bool - True on error, false if everything is okay
*/
bool Robot::getError() {
    return this->error;
}

/**
*   Set a timeout: if this timeout is reached, the error state will be setted to true
*   @param timeout long - Set the timeout in millis
*/
void Robot::setErrorTimeout(long timeout) {
    this->errorTimeout = timeout;
    this->errorTimer.reset();
    this->startErrorTimeout();
}

/**
*   Start the timer
*/
void Robot::startErrorTimeout() {
    this->errorTimer.start();
}

/**
*   Stop the timer
*/
void Robot::stopErrorTimeout() {
    this->errorTimer.stop();
}

/**
*  Check if a timeout is reached
* @param return bool - Timeout error is reached
*/
bool Robot::isTimeoutError() {
    if (this->getErrorMillis() > this->errorTimeout) {
        setError(true);
        return true;
    }
    return false;
}
