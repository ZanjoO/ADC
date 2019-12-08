#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

static int channel = 0;
static int speed = 500000; //WiringPi offers a range of integer values between 500k-32000k(Hz) which's respectively the CLK.
static int len = 0B10010; //Resolution of ADC or expected No. of bits.


//Tidy up
void sigHandler(void);

int main (void){
	
	
	//File to dump the data
	FILE *dataDump = NULL;
	printf("Create new Dump-file..\n");
	dataDump = fopen("./dataDump.txt", "a+"); //Creation, open for r/w at file end.
	
	if(dataDump == NULL){
		printf("Unable to create Dump-file\n");
		exit(1);
	}else
		printf("Done.\n");
		
	
	//Setup the SPI-Bus on CE0 and init. CLK.     	
	printf("Setup the SPI interface..\n");
	if(wiringPiSetup() || wiringPiSPISetup(channel, speed) < 0){
		printf("Failed to initialize the SPI-Bus.\n");
		exit(1);
	}
	else{
		printf("Done.\n");
	}
	/*Initialize the chip like given in the datasheet #CS/SHDN -> LOW; If started with CE0 on LOW u need to toggle first.
	 *By default on my PI3/4 the CS0 was on High. It's needed to invert it to initiate communication (MCP3008 Datasheet).
	 * -> gpio readall
	 **/
	
	
	digitalWrite(10, 0); //Pull CE0 to LOW to iniatiate communication
	delay(1);
	digitalWrite(12, 1); //Push MOSI to HIGH to institute startbit
	delay(5000);
	unsigned char data [10];
	data = "110010"
	int result = 0;
	while(1){
		//Pull CE0 to LOW!! Important for MCP3008
		result = wiringPiSPIDataRW(channel, data, len);
		printf("%d\n", result);
	}
}

/*
 *Author: S.P. Nuerenberg
 *Mod-Description: Software for communication between RaspberryPi and MCP3008-ADC IC.
 *Maybe more functionalities will follow
 *
 *
 * */
