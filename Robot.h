
/*
*   Robot.h - A library to control the stairsclimbing robot. 
*   Created by Sebastian G., Winterthur, April 04, 2022 
*/

#include "mbed.h"
#include "Config.h"

#ifndef ROBOT_H
#define ROBOT_H

class Robot {

    public:
        Robot();
        void stop();
        void slowStop();
        void driveForward();
        void driveBackward();
        void moveUp();
        void moveDown();
        int getProcedure();
        void setProcedure(int procedure);
        void setError(bool errorState);
        bool getError();
        void setErrorTimeout(double timeout); //Timeout in millis
        bool isError();
        void playSound(std::string name);

        
    private:
        double timeoutLimit;
        bool initialized;

};

#endif