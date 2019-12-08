#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


static int channel = 0;
static int speed = 1000000; //WiringPi offers a range of integer values between 500k-32000k(Hz) which's respectively the CLK.
static int len = 10; //Resolution of ADC or expected No. of bits.

int main (void){

	//init. spi
	int cs0 = 0; //Later needed for init the MCP3008
      	
	printf("Setup the SPI interface..");
	int spiErr = wiringPiSPISetup(channel, speed); //Setup the SPI-Bus CE0 and CLK
	if(spiErr == -1){
		printf("Failed to initialize the SPI-Bus.");
		return 1;
	}
	else{
		printf("Done.");
		delay(1000);
	}
	//Initialize the chip like given in the datasheet #CS/SHDN
	/*
	 *By default the CS0 is on High.
	 * */
	printf("Initialize MCP3008 communication..");
		
	printf("Done.");
	delay(1000);

	//Read stuff
	unsigned char *data = 0;
	while(1){
		printf( "%d", wiringPiSPIDataRW(channel, data, len));
	}

}

/*
 *Author: S.P. Nuerenberg
 *Mod-Description: Software for communication between RaspberryPi and MCP3008-ADC IC.
 *
 *
 *
 * */
