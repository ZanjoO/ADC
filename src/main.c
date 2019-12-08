#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

static int channel = 0;
static int speed = 1000000; //WiringPi offers a range of integer values between 500k-32000k(Hz) which's respectively the CLK.
static int len = 10; //Resolution of ADC or expected No. of bits.


//Tidy up
void sigHandler(void);

int main (void){
	int cs0 = 0; //Later needed for init the MCP3008
	
	//Data dumpfile
	FILE *dataDump = NULL;
	printf("Create new Dump-file..");
	dataDump = fopen("./dataDump.txt", "w"); //Creation
	
	if(dataDump == NULL){
		printf("Unable to create Dump-file");
		exit(EXIT_FAILURE);
	}else
		printf("Dump-file created.");
		
	
	//init. spi      	
	printf("Setup the SPI interface..");
	int spiErr = wiringPiSPISetup(channel, speed); //Setup the SPI-Bus on CE0 and CLK
	if(spiErr == -1){
		printf("Failed to initialize the SPI-Bus.");
		exit(EXIT_FAILURE);
	}
	else{
		printf("Done.");
		delay(1000);
	}
	/*Initialize the chip like given in the datasheet #CS/SHDN -> LOW; If started with CE0 on LOW u need to toggle first.
	 *By default on my PI3/4 the CS0 was on High. It's needed to invert it to initiate communication (MCP3008 Datasheet).
	 * -> gpio readall
	 **/
	printf("Initialize MCP3008 communication..");
	digitalWrite(10, LOW);
	
	printf("Done.");
	delay(1000);

	//Read stuff
	unsigned char *data = 0b0000000000;
	while(1){
		fputs(wiringPiSPIDataRW(channel, data, len), dataDump);
		printf( "%d", wiringPiSPIDataRW(channel, data, len));
	}
}

/*
 *Author: S.P. Nuerenberg
 *Mod-Description: Software for communication between RaspberryPi and MCP3008-ADC IC.
 *Maybe more functionalities will follow
 *
 *
 * */
