#pragma config(Sensor, S1,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S2,     sonar1,         sensorSONAR)
#pragma config(Sensor, S3,     colorSensor,    sensorColorNxtFULL)
#pragma config(Motor,  motorC,          mC,            tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
			TFileIOResult nBTCmdRdErrorStatus;
  		int nSizeOfMessage;
  		ubyte nRcvBuffer[kMaxSizeOfMessage];
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                           - Line Tracker -                                             *|
|*                                            ROBOTC on NXT                                               *|
|*                                                                                                        *|
|*  This program allows your taskbot to follow a line in reverse.                                         *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*    1)  The Light Sensor is attached to the back of the robot.                                          *|
|*    2)  Be sure to take readings of your Light Sensor over the light and dark areas.  Once you have     *|
|*        the values, add them and divide by 2 to find your threshold.  Then, use your threshold as a     *|
|*        comparison in your program.                                                                     *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port A                motorB              NXT                 Right motor                         *|
|*    Port B                 motorC              NXT                 Left motor                          *|
|*    Port 1                  lightSensor         Light Sensor        front mounted                        *|
|*	  Port 2									sonar1							ultrasonic sensor		front mounted												*|
|*		Port 3									colorSensor					colorSensor					front mounted
\*---------------------------------------------------------------------------------------------------4246-*/


task main()
{

	wait1Msec(50);                        // The program waits 50 milliseconds to initialize the light sensor.

	while(true)                           // Infinite loop
	{
		motor[motorC] = 20;
		if (SensorValue[sonar1] < 20) {
			motor[motorA] = -50;
			motor[motorB] = 50;
			wait1Msec(450);
		}

		else if (SensorValue[colorSensor] ==  BLACKCOLOR) {
			motor[motorA] = 0;
			motor[motorB] = 0;
				int j = 0;


  while (true)
  {
    // Check to see if a message is available

    nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage){
      nSizeOfMessage = kMaxSizeOfMessage;
    }
    if (nSizeOfMessage > 0){
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	string s = "";
    	stringFromChars(s, (char *) nRcvBuffer);
    	displayCenteredBigTextLine(4, s);
    	if (s == "UP") {
    		motor[motorA] = -30;
    		motor[motorB] = -30;
    		wait1Msec(250);
    	}
    	else if (s == "LEFT") {
    		motor[motorA] = -50;
    		motor[motorB] = -20;
    		wait1Msec(500);
    	}
    	else if (s == "RIGHT") {
    		motor[motorA] = 40;
    		motor[motorB] = -20;
    		wait1Msec(400);
    	}
    	break;
    }
    wait1Msec(100);
    j++;
    if (j > 30) {
    	motor[motorA] = -50;
    	motor[motorB] = -50;
    	wait1Msec(200);
    	break;
    }
  }
 	 }
		else {
		displayCenteredBigTextLine(4, "%d", SensorValue[lightSensor]);
			motor[motorA] = -20 + ((60 - SensorValue[lightSensor]));
			motor[motorB] = -20 - ((60 - SensorValue[lightSensor]));
			/*if (SensorValue[lightSensor] < 55)  // If the Light Sensor reads a value less than 45:
			{
			motor[motorA] = 8;                  // Motor B is run at a 60 power level.
			motor[motorB] = -18;                  // Motor C is run at a 20 power level.
			}
			else if (SensorValue[lightSensor] < 63) {
				motor[motorA] = -30;
				motor[motorB] = -30;
			}
			else                               // If the Light Sensor reads a value greater than or equal to 45:
			{
			motor[motorA] = -30;
			motor[motorB] = 5;

			}*/
			}
		}
}
