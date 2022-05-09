/*
*   Robot.cpp - A library to control the stairsclimbing robot. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/
#include <stdio.h>
#include "Robot.h"



Robot::Robot() {
    /*
    *   Start the timer
    */
    this->mainTimer.start();
    this->taskTimer.start();
    this->internalTaskTimer.start();
    this->errorTimer.start();
    /*
    *   Setup motors
    */
    this->M_MB = new FastPWM(PIN_MOTOR_MAINBODY);
    this->M_SB = new FastPWM(PIN_MOTOR_SIDEBODY);
    this->M_Z  = new FastPWM(PIN_MOTOR_ZAXIS);
    this->M_EN = new DigitalOut(PIN_MOTOR_ENABLE);
    this->M_MB->period(0.00005);
    this->M_SB->period(0.00005);
    this->M_Z->period(0.00005);
    
    this->motorStop();
    /*
    *   Setup switches
    */
    this->SW_IR_BACK = new DigitalIn(PIN_SWITCH_IR_BACK);
    this->SW_IR_MIDDLE = new DigitalIn(PIN_SWITCH_IR_MIDDLE);
    this->SW_IR_FRONT = new DigitalIn(PIN_SWITCH_IR_FRONT);
    this->SW_Z_MIN = new DigitalIn(PIN_SWITCH_Z_MIN);
    this->SW_Z_MAX = new DigitalIn(PIN_SWITCH_Z_MAX);
    this->SW_START = new InterruptIn(PIN_SWITCH_START);

    this->SW_Z_MIN->mode(PullUp);
    this->SW_Z_MAX->mode(PullUp);
    this->SW_START->mode(PullUp);

    /*
    *   Setup sensors
    */
    this->SE_IR = new AnalogIn(PIN_SENSOR_DISTANCE);

    /*
    *   Setup error management
    */
    this->LED_error = new DigitalOut(PIN_LED_ERROR);
    this->LED_warning = new DigitalOut(PIN_LED_WARNING);
    this->LED_running = new DigitalOut(PIN_LED_RUN);
    this->setError(false);


} 

/**
*   Reset the task timer
*/
void Robot::resetTaskTimer() {
    this->taskTimer.reset();
}

/**
*   Disable all stepper and run in endless while
*/
void Robot::emergencyStop() {
    this->motorStop();
    while(true) printf("Emergency stop!");
}

/**
*   Get the actual time in milliseconds since the last time reset
*   @return long long - get the actual milliseconds
*/
long long Robot::getMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->mainTimer.elapsed_time()).count();
}


/**
*   Get the actual time in milliseconds since the last time reset
*   @return long long - get the actual milliseconds since the last timer reset
*/
long long Robot::getTaskMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->taskTimer.elapsed_time()).count();
}


/**
*   Get the actual time in milliseconds since the last time reset
*   @return long long - get the actual milliseconds since the last timer reset
*/
long long Robot::getInternalTaskMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->internalTaskTimer.elapsed_time()).count();
}

/**
*   Get the actual time of errorTimer in milliseconds since the last time reset
*   @return long long - get the actual milliseconds of errorTimer since the last timer reset
*/
long long Robot::getErrorMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(this->errorTimer.elapsed_time()).count();
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
*   @return int - the setted procedureCode
*/
int Robot::getProcedureCode() {
    return this->procedureCode;
}

/**
*   Set the enabled state of the motors
*   @param en Bool - Set the enabled state of the motors: true -> enabled, false -> disabled
*/
void Robot::enableMotors(bool en) {
    if (en && !this->error) 
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
    //Store the speeds at beginning of the slow down process
    double startSpeeds[] = {
        this->M_MB->read(),
        this->M_SB->read(),
        this->M_Z->read()
    };
    // Reset the timer
    this->internalTaskTimer.reset();
    //Define the actual time
    long long actualTime = 0;
    //loop while slowing down
    while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
        M_MB->write((0.5-startSpeeds[0])/(double)MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[0]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
        M_SB->write((0.5-startSpeeds[1])/(double)MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[1]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
        M_Z->write((0.5-startSpeeds[2])/(double)MOTOR_SLOW_STOP_DURATION*actualTime+startSpeeds[2]); // Speed(t) = (0.5f - startSpeed) / slow down time * t + startSpeed 
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
*   Set an warning state and show it over an LED
*   @param warningState bool - If warning, set true, if not set false
*/
void Robot::setWarning(bool warningState) {
    *(this->LED_warning) = (warningState ? 1 : 0);
}

/**
*   Ge the actual error state
*   @return bool - True on error, false if everything is okay
*/
bool Robot::getError() {
    return this->error;
}

/**
*   Set a timeout: if this timeout is reached, the error state will be setted to true
*   @param timeout long - Set the timeout in millis
*/
void Robot::setErrorTimeout(long long timeout) {
    this->errorTimeout = timeout;
    this->errorTimer.reset();
}

/**
*  Check if a timeout is reached
* @return bool - Timeout error is reached
*/
bool Robot::isTimeoutError() {
    if (this->getErrorMillis() > this->errorTimeout) {
        setError(true);
        return true;
    }
    return false;
}

/**
*   Start to drive the mainbody and sidebody motors
*   @param dir short - 1 = forward, 0 = backward
*/
void Robot::driveH(short dir) {
    if (!this->error) {
        //make sure that all motors are stopped
        motorStop();
        //Enable motors and set speed
        this->enableMotors(true);
        //Store the speed at beginning of the slow down process
        double startSpeed = 0.5f;
        // Reset the timer
        this->internalTaskTimer.reset();
        //Define the actual time
        long long actualTime = 0;
        //loop while speed up
        while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
            M_MB->write(0.5f+pow(-1,MOTOR_DIRECTION_MAINBODY)*pow(-1,dir)*MOTOR_PWM_MAINBODY/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
            M_SB->write(0.5f+pow(-1,MOTOR_DIRECTION_SIDEBODY)*pow(-1,dir)*MOTOR_PWM_SIDEBODY/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
        }
    }
}

/**
*   Start to drive the mainbody motor
*   @param dir short - 1 = forward, 0 = backward
*/
void Robot::driveMB(short dir) {
    if (!this->error) {
        //make sure that all motors are stopped
        motorStop();
        //Enable motors and set speed
        this->enableMotors(true);
        //Store the speed at beginning of the slow down process
        double startSpeed = 0.5f;
        // Reset the timer
        this->internalTaskTimer.reset();
        //Define the actual time
        long long actualTime = 0;
        //loop while speed up
        while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
            M_MB->write(0.5f+pow(-1,MOTOR_DIRECTION_MAINBODY)*pow(-1,dir)*MOTOR_PWM_MAINBODY/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
        }
    }
}

/**
*   Start to drive the sidebody motor
*   @param dir short - 1 = forward, 0 = backward
*/
void Robot::driveSB(short dir) {
    if (!this->error) {
        //make sure that all motors are stopped
        motorStop();
        //Enable motors and set speed
        this->enableMotors(true);
        //Store the speed at beginning of the slow down process
        double startSpeed = 0.5f;
        // Reset the timer
        this->internalTaskTimer.reset();
        //Define the actual time
        long long actualTime = 0;
        //loop while speed up
        while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
            M_SB->write(0.5f+pow(-1,MOTOR_DIRECTION_SIDEBODY)*pow(-1,dir)*MOTOR_PWM_SIDEBODY/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
        }
    }
}

/**
*   Start to drive the sidebody motor
*   @param dir short - 1 = forward, 0 = backward
*/
void Robot::slowSB(short dir) {
    if (!this->error) {
        //make sure that all motors are stopped
        motorStop();
        //Enable motors and set speed
        this->enableMotors(true);
        //Store the speed at beginning of the slow down process
        double startSpeed = 0.5f;
        // Reset the timer
        this->internalTaskTimer.reset();
        //Define the actual time
        long long actualTime = 0;
        //loop while speed up
        while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
            M_SB->write(0.5f+pow(-1,MOTOR_DIRECTION_SIDEBODY)*pow(-1,dir)*SLOW_PWM_SIDEBODY/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
        }
    }
}

/**
*   Start to drive the Z-axis motor
*   @param dir short - 1 = forward, 0 = backward
*/
void Robot::driveZ(short dir) {
    if (!this->error) {
        //make sure that all motors are stopped
        motorStop();
        //Enable motors and set speed
        this->enableMotors(true);
        //Store the speed at beginning of the slow down process
        double startSpeed = 0.5f;
        // Reset the timer
        this->internalTaskTimer.reset();
        //Define the actual time
        long long actualTime = 0;
        //loop while speed up
        while ((actualTime = this->getInternalTaskMillis()) < MOTOR_SLOW_STOP_DURATION) {
            M_Z->write(0.5f+pow(-1,MOTOR_DIRECTION_ZAXIS)*pow(-1,dir)*MOTOR_PWM_ZAXIS/MOTOR_SLOW_STOP_DURATION*actualTime); // Speed(t) = zeroSpeed + fullSpeed / slow down time * t
        }
    }
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getMinZSwitch() {
    return this->SW_Z_MIN->read();
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getMaxZSwitch() {
    return this->SW_Z_MAX->read();
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getFrontIRSwitch() {
    return this->SW_IR_FRONT->read() ? 0 : 1;
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getMiddleIRSwitch() {
    return this->SW_IR_MIDDLE->read() ? 0 : 1;
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getBackIRSwitch() {
    return this->SW_IR_BACK->read() ? 0 : 1;
}

/**
*   Get the switch value
*   @return short - digital value of the switch 
*/
short Robot::getStartSwitch() {
    return this->SW_START->read();
}
/**
*   Get the sensor value
*   @return double - analog value of the sensor 
*/
float Robot::getIRSensorValue() {
    return this->SE_IR->read();
}

bool Robot::getInitPos(){
    if(!(this->getFrontIRSwitch()) && this->getMiddleIRSwitch() && this-> getBackIRSwitch() && this->getMaxZSwitch() && !(this->getMinZSwitch())){
        return true;
    }else{
        return false;
    }
}