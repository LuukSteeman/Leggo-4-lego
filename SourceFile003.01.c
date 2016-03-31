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

void app_move(string s, bool bocht){
		int tmp_right;
		int tmp_left;
		if (bocht == true) {
			tmp_right = -180;
			tmp_left = -220;
		}
		else {
			tmp_left = -45;
			tmp_right = -45;
		}

  	if (s == "UP"){
			nMotorEncoder[motorA] = 0;
			//nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorA] > -45) {
				motor[motorA] = -30;
				motor[motorB] = -30;
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "LEFT") {
			nMotorEncoder[motorA] = 0;
			while (nMotorEncoder[motorA] > tmp_left) {
				motor[motorA] = -20;
				motor[motorB] = 0;
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "RIGHT") {
			nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorB] > tmp_right) {
				motor[motorA] = 0;
				motor[motorB] = -20;
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "DOWN") {
			nMotorEncoder[motorA] = 0;
			//nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorA] < 45) {
				motor[motorA] = 30;
				motor[motorB] = 30;
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "STOP") {
		motor[motorA] = 0;
		motor[motorB] = 0;
	}

}

task main()
{

	int timer_release = 1;
	int app_timeout = 3000;
	int stop_functie = 0;
	bool intersection = false;


	string app_message = "";
	wait1Msec(50);                        // The program waits 50 milliseconds to initialize the light sensor.
	while(true)                           // Infinite loop
	{
		/** check if there is a message available from app */
    nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage){
      nSizeOfMessage = kMaxSizeOfMessage;
    }
  	/** store the message of the app into a variable*/
    if (nSizeOfMessage > 0) {
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	stringFromChars(app_message, (char *) nRcvBuffer);
    	displayCenteredBigTextLine(4, app_message);
    	if (app_message == "FIRE") {
    		if (stop_functie == 0) {
    			stop_functie = 1;
    			app_message = "STOP";
    			app_move(app_message, false);
    			continue;
    		}
    		else if (stop_functie == 1){
    			//motor[motorA] = 0;
    			//motor[motorB] = 0;
    			stop_functie = 0;
    			timer_release = 1;
    		}
    	}
	  }

		/** check the sonar sensor and stop the robot if it sees sth */
		if (stop_functie == 0 && SensorValue[sonar1] < 20) {
			for(int i = 0; i < 20; i++){
							motor[motorA] = -20 + (1 * i);
							motor[motorB] = -20 + (1 * i);
							wait1Msec(1000);
				}
			}
		/** check the color sensor if it's black stop the robot */
		else if (stop_functie == 0 && SensorValue[colorSensor] ==  BLACKCOLOR) {
				playSound(soundException);
			for (int i=-20; i < 0; i++) {
				motor[motorA] = i;
				motor[motorB] = i;
				wait1Msec(800);
			}
			intersection = true;

		}

		/** if the robot is at intersection, wait for a reactie from app
				The time-out is max "app_timeout" */
		else if (stop_functie == 0 && intersection == true) {

			/* Reset Timer1 value (only the first time (when the timer is free)
				and check if its reached app_timeout) */
			if (timer_release == 1){
					ClearTimer(T1);
					timer_release = 0;
				}

			if (timer_release == 0 && time1[T1] < app_timeout) {
	    		if (nSizeOfMessage > 0){
	    			app_message = app_message;
	    			app_move(app_message, true)
	    			stop_functie = 1;
	    			/* Release the timer */
	    			timer_release = 1;
			    }
			}
			else if (timer_release == 0 && time1[T1] > app_timeout){
				nMotorEncoder[motorA] = 0;
				while (nMotorEncoder[motorA] >  -180) {
					motor[motorA] = -20;
					motor[motorB] = -20;
				}
				/* Release the timer */
				timer_release = 1;
			}
		}
	else if (stop_functie == 0 && intersection == false) {
		displayCenteredBigTextLine(4, "%d", SensorValue[lightSensor]);
			motor[motorA] = -20 + ((55 - SensorValue[lightSensor]));
			motor[motorB] = -20 - ((55 - SensorValue[lightSensor]);
		}
	else if (stop_functie == 1) {
		app_move(app_message, false);
		app_message = "";
		}
}
}
