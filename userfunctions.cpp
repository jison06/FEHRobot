#include "userfunctions.h"
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHIO.h>



UserFunctions::UserFunctions()
{

}

void LineFollower(AnalogInputPin leftSensor, AnalogInputPin centerSensor, AnalogInputPin rightSensor, FEHMotor rightMotor, FEHMotor leftMotor)
{
    followingStates lineState = allOnLine;
    bool offLine = (lineState == off);
    while (!offLine)
    {
        bool rightSensorOffLine = (rightSensor.Value() <= 1.83 && centerSensor.Value() > 2.81 && leftSensor.Value() > 2.65);
        bool rightAndCenterOffLine = (rightSensor.Value() <= 1.82 && centerSensor.Value() <= 2.417 && leftSensor.Value() > 2.65);
        bool leftOffLine = (rightSensor.Value() > 2.7 && centerSensor.Value() > 2.81 && leftSensor.Value() <= 1.958);
        bool leftAndCenterOffLine = (rightSensor.Value() > 2.7 && centerSensor.Value() <= 2.417 && leftSensor.Value() <= 1.958);
        bool allOffLine = (rightSensor.Value() < 1.83 && centerSensor.Value() <= 2.417 && leftSensor.Value() <= 1.958);

        switch (lineState)
        {
        case allOnLine:
            rightMotor.SetPercent(-1.08 * (forwardMotorPercent));
            leftMotor.SetPercent(forwardMotorPercent);
            break;
        case lineOnRight:
            rightMotor.SetPercent(-1.08 * (forwardMotorPercent / 2.0));
            leftMotor.SetPercent(forwardMotorPercent);
            break;
        case lineOnLeft:
            leftMotor.SetPercent(forwardMotorPercent / 2.0);
            rightMotor.SetPercent(-1.08 * (forwardMotorPercent));
            break;
        case off:
            LCD.WriteLine("All sensors off the line");
            break;
        }

        if (rightSensorOffLine || rightAndCenterOffLine) //line is to the right
        {
            lineState = lineOnLeft;
        }
        else if (leftOffLine || leftAndCenterOffLine) //Line is to the left
        {
            lineState = lineOnRight;
        }
        else if (allOffLine) //If all sensors are off the line exit the loop
        {
            lineState = off;
        }
        else
        {
            lineState = allOnLine;
        }
    }
}
