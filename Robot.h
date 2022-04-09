
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
        Robot(); //TODO
        void motorStop();
        void slowMotorStop();
        void enableMotors(bool en);
        void driveMB(short dir); //TODO
        void driveSB(short dir); //TODO
        void driveZ(short dir); //TODO;
        int getProcedureCode();
        void setProcedureCode(int procedureCode);
        void setError(bool errorState); 
        bool getError(); 
        void setErrorTimeout(long timeout);
        void stopErrorTimeout(); 
        void startErrorTimeout();
        long getErrorMillis(); 
        bool isTimeoutError();
        void playSound(std::string name); //TODO
        long getMillis(); 
        void resetTimer();
        short getMinZSwitch(); //TODO
        short getMaxZSwitch(); //TODO
        short getFrontIRSwitch(); //TODO
        short getBackIRSwitch(); //TODO
        double getIRSensorValue(); //TODO
        
    private:

        /*
        *   General variables
        */
        int procedureCode = 0;
        bool error = false;
        long errorTimeout = 0;
        
        /*
        * Timer variables
        */
        Timer taskTimer; 
        Timer errorTimer; 

        /*
        * LED variables
        */
        DigitalOut *LED_error;
        DigitalOut *LED_running;

        /*
        * Motor variables
        */
        FastPWM *M_MB;
        FastPWM *M_SB;
        FastPWM *M_Z;
        DigitalOut *M_EN;
};

#endif