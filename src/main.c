#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

static int channel = 0;
static int speed = 1000000;

int main (void){
	
	int err = wiringPiSPISetup(channel, speed)
	if(err == -1){
		printf("Failed to initialize the SPIIi");
	}



}

