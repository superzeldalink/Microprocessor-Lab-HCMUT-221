#include "header.h"
#include "lib/lcd_lib.h"
#include "lib/nyan.h"

// BUZZER STUFFS
alt_u32 millis() {
	alt_u32 timestamp = alt_timestamp();
	float t = timestamp *1.0 / alt_timestamp_freq();
	return t * 1000;
}

void tone(int frequency, int duration) {
	unsigned long startTime = millis();
	unsigned long halfPeriod = 1000000L / frequency / 2;
	while (millis() - startTime < duration)	{
		WRITEDATA(BUZZER_BASE, 1);
		usleep(halfPeriod);
		WRITEDATA(BUZZER_BASE, 0);
		usleep(halfPeriod);
	}
}

// STEPPER STUFFS
#define MAX_RPM 15
char currentStep = 0;
int rpm = 1;
char dir = -1;
char pattern[8] = {0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001};

int rpm2step(int rpm){
	return 64*rpm;
}

alt_u32 stepper_alarm(void *context) {
	currentStep+= dir;
	WRITEDATA(STEPPER_BASE, pattern[currentStep]);

	if (currentStep > 7)
		currentStep = 0;
	else if (currentStep < 0)
		currentStep = 7;

	return alt_ticks_per_second() / rpm2step(rpm);
}

// LCD STUFFS
void PrintSpeed() {
	lcd_setcursor(1, 5);
	char stringNum[2];
	sprintf(stringNum, "%d", rpm);
	lcd_printtext(stringNum);
	lcd_printtext(" ");
}

void PrintDir() {
	lcd_setcursor(1, 15);
	if(dir == -1)
		lcd_printtext("CW ");
	else
		lcd_printtext("CCW");
}

void PrintHW() {
	lcd_setcursor(0, 0);
	lcd_printtext("Hello World!!!");
}

char hw_state = 1;
alt_u32 hw_alarm(void *context) {
	if (hw_state == 0) {
		PrintHW();
		hw_state = 1;
	}
	else {
		lcd_setcursor(0, 0);
		lcd_printtext("              ");
		hw_state = 0;
	}

	return alt_ticks_per_second();
}

// NYAN STUFFS
int nyan_cursor_x = 0;
int nyan_speed = 2;
alt_u32 nyan_loop(void *context) {
	lcd_setcursor(2, 0);
	lcd_printtext("                    ");

	for (int i = 0; i < 3; i++) {
		if (nyan_cursor_x > 19)
			nyan_cursor_x = 0;
		lcd_setcursor(2, nyan_cursor_x);
		lcd_senddata(i);
		nyan_cursor_x++;
	}

	nyan_cursor_x -= 2;

	if (nyan_cursor_x < 0)
		nyan_cursor_x = 0;
	return alt_ticks_per_second() / nyan_speed;
}

char wavePattern = 0;
alt_u32 waveLoop(void *context) {
	lcd_sendcmd(0x40 + 48);
	if(wavePattern % 2)
		for (int i = 0; i < 16; i++) lcd_senddata(wave[i]);
	else {
		for (int i = 8; i < 16; i++) lcd_senddata(wave[i]);
		for (int i = 0; i < 8; i++) lcd_senddata(wave[i]);
	}

	wavePattern++;
	return alt_ticks_per_second() / nyan_speed;
}

void InitSmallNyan(){
	lcd_sendcmd(0x40);
	for (int i = 0; i < 8; i++) lcd_senddata(cc1[i]);
	for (int i = 0; i < 8; i++) lcd_senddata(cc2[i]);
	for (int i = 0; i < 8; i++) lcd_senddata(cc3[i]);
}

void PrintLargeNyan(){
	lcd_sendcmd(0x40);
	for (int i = 0; i < 48; i++) lcd_senddata(nyan[i]);
	lcd_sendcmd(0x40 + 48);
	for (int i = 0; i < 16; i++) lcd_senddata(wave[i]);

	lcd_setcursor(2,0);
	for(int i = 0; i < 5; i++){
		lcd_senddata(6);
		lcd_senddata(7);
	}
	lcd_setcursor(3,0);
		for(int i = 0; i < 5; i++){
			lcd_senddata(6);
			lcd_senddata(7);
		}

	lcd_setcursor(2,9);
	for(int i = 0; i < 3; i++){
		lcd_senddata(i);
	}
	lcd_setcursor(3,9);
	for(int i = 3; i < 6; i++){
		lcd_senddata(i);
	}

	lcd_setcursor(2,14);
	lcd_printtext("NYAAN");

	lcd_setcursor(3,15);
	lcd_printtext("CAT");
}

// MAIN PROGRAM
int main() {
	static alt_alarm alarm, alarm2, nyan_alarm;

	lcd_init();		// Init LCD
	PrintHW();		// Print "Hello World!!!"

	// Print Speed and direction
	lcd_setcursor(1, 0);
	lcd_printtext("RPM:      Dir:  ");
	PrintSpeed();
	PrintDir();

	// Test alarm
	if (alt_alarm_start(&alarm, alt_ticks_per_second(), NULL, NULL) < 0 || alt_alarm_start(&alarm2, alt_ticks_per_second(), NULL, NULL) < 0) {
		printf("Error!\n");
		return 0;
	}

	alt_alarm_stop(&alarm);
	alt_alarm_stop(&alarm2);

	// Start nyancat
	//InitNyan();
	alt_alarm_start(&nyan_alarm, alt_ticks_per_second() / nyan_speed, waveLoop, NULL);
	PrintLargeNyan();

	// Variables
	int switch_prev_state = 0;
	int thisNote = 0;
	char musicOn = 0;

	// READYY!!
	printf("Ready\n");

	while(1){
		// Check switches
		int sw = READDATA(SWITCH_BASE) & 7;
		if (sw != switch_prev_state){

			alt_alarm_stop(&alarm);
			alt_alarm_stop(&alarm2);

			if ((sw & 0b100) != (switch_prev_state & 0b100)){
				if ((sw & 0b100) == 0b100){
					thisNote = 0;
					musicOn = 1;
					if (alt_timestamp_start() < 0)
						printf("Timestamp ERROR!");
				}
				else{
					WRITEDATA(BUZZER_BASE, 0);
					musicOn = 0;
				}
			}

			if ((sw & 2) == 2)
				alt_alarm_start(&alarm, alt_ticks_per_second() / rpm2step(rpm), stepper_alarm, NULL);

			if ((sw & 1) == 1)
				alt_alarm_start(&alarm2, alt_ticks_per_second(), hw_alarm, NULL);
			else
			{
				hw_state = 1;
				PrintHW();
			}

			switch_prev_state = sw;
		}

		// Check buttons
		char btn = READDATA(KEY_BASE);
		if (btn != 15) {
			if (btn == 0b1110)
				rpm++;
			else if (btn == 0b1101)
				rpm--;
			else if (btn == 0b1011)
				dir*=-1;
			else if (btn == 0b0111)
				for (int i = 0; i < 4000; i++){
					WRITEDATA(STEPPER_BASE, pattern[i % 8]);
					usleep(5000);
				}

			// CLAMP SPEED
			if (rpm > MAX_RPM)
				rpm = MAX_RPM;
			else if (rpm < 1)
				rpm = 1;

			PrintSpeed();
			PrintDir();

			alt_alarm_stop(&alarm);
			if ((sw & 2) == 2)
				alt_alarm_start(&alarm, alt_ticks_per_second() / rpm2step(rpm), stepper_alarm, NULL);

			while (READDATA(KEY_BASE) != 15);
		}

		// Music stuffs :))
		if(musicOn) {
			// MUSIC
			if(thisNote >= sizeof(melody) / sizeof(melody[0])){
				musicOn = 0;
				thisNote = 0;
			}
			else {
				/*
				  to calculate the note duration, take one second divided by the note type.
				  e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
				 */
				int noteDuration = 1000 / noteDurations[thisNote];
				tone(melody[thisNote] * 10 + 4, noteDuration);

				/*
				  to distinguish the notes, set a minimum time between them.
				  the note's duration + 30% seems to work well:
				 */
				int pauseBetweenNotes = noteDuration *1.30;
				usleep(pauseBetweenNotes *1000);

				WRITEDATA(BUZZER_BASE, 0);	//stop the tone playing:
				thisNote++;
			}
		}
	}

	return 0;
}
