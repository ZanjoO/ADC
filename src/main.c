#include <stdio.h>
#include <stdlib.h>
//At this point thx to Gordon Henderson for this awesome lib.
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS 10 //Chip select Pin
#define CHAN 0 //Channel 0/1 
#define SPEED 1350000 //Bus speed
#define LEN 3 //Length of expected bytes

double doVolts( unsigned char data[] ){
	double res = ((( data[1] & 0x03 ) << 8 ) | data[2] );

	return (( res * 3.3 ) / 1024);
}

int doDecimal( unsigned char data[] ){
	int result;
	result = ((data[1] & 0x03)<<8) | data[2];

	return result;
}

void doWav(unsigned char inputArray[]);

int main (void){
	
	
	//File to dump the data
	FILE *dataDump;
	printf( "Create new Dump-file..\n" );
	dataDump = fopen( "./dataDump.txt", "a+" ); //Creation, open for r/w at file end.
	
	if( dataDump == NULL ){
		printf( "Unable to create Dump-file\n" );
		exit(1);
	}else
		printf( "Done.\n" );
		
	
	//Setup the SPI-Bus on CE0 and init. CLK.     	
	printf( "Setup the SPI interface..\n" );
	if( wiringPiSetup() || wiringPiSPISetup( CHAN, SPEED ) < 0 ){
		printf( "Failed to initialize the SPI-Bus.\n" );
		exit( 1 );
	}
	else{
		printf( "Done.\n" );
		printf( "Start write to file.. \n" );
	}
	
	digitalWrite( CS, 1 );
	digitalWrite( CS, 0 );
	unsigned char data[3];
	while(1){
		data[0] = 0x01;
		data[1] = 0x80;
		data[2] = 0x00;

		wiringPiSPIDataRW( CS, data, LEN );
	
		fprintf( dataDump, "%d, %f\n", doDecimal(data), doVolts(data) );
	}
}
/*
 *Author: S.P. Nuerenberg
 *Mod-Description: Software for communication between RaspberryPi and MCP3008-ADC IC.
 *Maybe more functionalities will follow
 *
 *
 * */
