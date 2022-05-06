
/*
*   Robot.h - A library to control the stairsclimbing robot. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/

#include "mbed.h"
#include "MyConfig.h"
#include "PM2_Libary.h"



#ifndef ROBOT_H
#define ROBOT_H

class Robot {

    public:
        Robot();
        void motorStop();
        void slowMotorStop();
        void enableMotors(bool en);
        void driveMB(short dir);
        void driveSB(short dir);
        void driveH(short dir);
        void driveZ(short dir);
        int getProcedureCode();
        void setProcedureCode(int procedureCode);
        void setError(bool errorState); 
        void setWarning(bool warningState); 
        bool getError(); 
        void setErrorTimeout(long long timeout);
        void resetTaskTimer();
        long long getTaskMillis();
        long long getErrorMillis(); 
        bool isTimeoutError();
        long long getMillis(); 
        short getMinZSwitch();
        short getMaxZSwitch();
        short getFrontIRSwitch();
        short getMiddleIRSwitch();
        short getBackIRSwitch();
        short getStartSwitch();
        double getIRSensorValue();
        void emergencyStop();
        bool getInitPos();
        InterruptIn *SW_START;
        
        //void playSound(std::string name); //TODO
        
    private:

        /*
        *   General variables
        */
        int procedureCode = 0;
        bool error = false;
        long long errorTimeout = 0;
        
        /*
        * Timer variables
        */
        Timer mainTimer; 
        Timer taskTimer; 
        Timer internalTaskTimer; 
        Timer errorTimer; 
    
        long long getInternalTaskMillis();

        /*
        * LED variables
        */
        DigitalOut *LED_error;
        DigitalOut *LED_warning;
        DigitalOut *LED_running;

        /*
        * Motor variables
        */
        FastPWM *M_MB;
        FastPWM *M_SB;
        FastPWM *M_Z;
        DigitalOut *M_EN;

        /*
        * Switch inputs
        */
        DigitalIn *SW_IR_BACK;
        DigitalIn *SW_IR_MIDDLE;
        DigitalIn *SW_IR_FRONT;
        DigitalIn *SW_Z_MIN;
        DigitalIn *SW_Z_MAX;
        

        /*
        *   Sensor inputs
        */
        AnalogIn *SE_IR;
};

#endif
