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
			tmp_right = -60;
			tmp_left = -90;
			//playSound(soundBeepBeep);
		}
		else {
			tmp_left = -45;
			tmp_right = -45;
			//playSound(soundBeepBeep);
		}

  	if (s == "UP"){
			nMotorEncoder[motorA] = 0;
			//nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorA] > -45) {
				motor[motorA] = -30;
				motor[motorB] = -30;
				playSound(soundBeepBeep);
				clearSounds();
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "LEFT") {
			nMotorEncoder[motorA] = 0;
			while (nMotorEncoder[motorA] > tmp_left) {
				motor[motorA] = -20;
				motor[motorB] = 20;
				playSound(soundBeepBeep);
				clearSounds();
			}
			motor[motorA] = 0;
			motor[motorB] = 0;

	}
	else if (s == "RIGHT") {
			nMotorEncoder[motorB] = 0;
			while (nMotorEncoder[motorB] > tmp_right) {
				motor[motorA] = 20;
				motor[motorB] = -20;
				playSound(soundBeepBeep);
				clearSounds();
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
				playSound(soundBeepBeep);
				clearSounds();
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
	}
	else if (s == "STOP") {
		motor[motorA] = 0;
		motor[motorB] = 0;
	}

}

float power(float number,int n)
{
  float Z=number;
  for (int i=1;i<n;i++)
    Z*=number;
  return Z;
}


task main()
{

	int timer_release = 1;
	int app_timeout = 3000;
	int stop_functie = 1;
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
    	if (app_message == "FIRE" && timer_release == 1) {
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
		if (stop_functie == 0 && intersection == false && SensorValue[sonar1] < 20) {
				int i = -15;
					while (i < 0) {
						clearSounds();
						playSound(soundBlip);
						motor[motorA] = i;
						motor[motorB] = i;
						i +=  4;
						wait1Msec(300);
				}
			motor[motorA] = 0;
			motor[motorB] = 0;
		}
		/** check the color sensor if it's black stop the robot */
		else if (stop_functie == 0 && SensorValue[colorSensor] ==  BLACKCOLOR && SensorValue[lightSensor] < 50) {
				int i = -15;
				while (i < 0) {
					clearSounds();
					playSound(soundException);
					motor[motorA] = i;
					motor[motorB] = i;
					i +=  4;
					wait1Msec(300);
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
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
	    			/* Release the timer */
	    			timer_release = 1;
	    			intersection = false;
	    			stop_functie = 1;
	    			if (app_message == "FIRE") {
	    				stop_functie = 0;
	    			}
	    			else if (app_message != "UP") {
	    				app_move(app_message, true);
	    			}
			    }
			}
			else if (timer_release == 0 && time1[T1] > app_timeout){
				intersection = false;
				/* Release the timer */
				timer_release = 1;
			}
		}
	else if (stop_functie == 0 && intersection == false) {
		displayCenteredBigTextLine(4, "%d", SensorValue[lightSensor]);
			motor[motorA] = -22 + (power((float)(60 - SensorValue[lightSensor]) / 12, 3) * 12);
			motor[motorB] = -22 - (power((float)(60 - SensorValue[lightSensor]) / 12, 3) * 12);
			nVolume = 2;
			playSound(soundBeepBeep);
		}
	else if (stop_functie == 1) {
		app_move(app_message, false);
		app_message = "";
		clearSounds();
		}
}
}
