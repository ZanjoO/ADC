#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


#define MOSI 12
#define MISO 13
#define CS 10 //Chip select Pin
#define CHAN 0 //Channel 0/1 
#define SPEED 1000000 //Bus speed
#define LEN 3 //Length of expected bytes

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
	if(wiringPiSetup() || wiringPiSPISetup(CHAN, SPEED) < 0){
		printf("Failed to initialize the SPI-Bus.\n");
		exit(1);
	}
	else{
		printf("Done.\n");
	}
	/*Initialize the chip like given in the datasheet #CS/SHDN -> LOW; If started with CE0 on LOW u need to toggle first.
	 *By default on my PI3/4 the CE0 was on High. It's needed to invert it to initiate communication (MCP3008 Datasheet).
	 * -> gpio readall
	 **/
	
	//to transmit : 0x01(start bit) 0x80(Single mode inclusive channel select)
	//Make sure that toggle from 1 to 0

	unsigned char data[3];
	while(1){
		//Pull CS to LOW to iniatiate communication
		data[0] = 0x01;
		data[1] = 0x80;
	       	data[2] = 0x00;
		digitalWrite(CS, 0);	
		wiringPiSPIDataRW(CS, data, LEN);
		digitalWrite(CS, 1);
		printf("%d\n", data[0]);
		printf("%d\n", data[1]);
		printf("%d\n", data[2]);
		printf("%d\n", doVolts(data[1], data[2]));
	}
}

short doVolts(unsigned char msbArray, unsigned char lsbArray){
	
	return ((int) (((msbArray & 0x03) << 8) + lsbArray));
}

int doBits(unsigned char msbArray, unsigned char lsbArray){
	return 0;
}
/*
 *Author: S.P. Nuerenberg
 *Mod-Description: Software for communication between RaspberryPi and MCP3008-ADC IC.
 *Maybe more functionalities will follow
 *
 *
 * */
