#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS 10 //Chip select Pin
#define CHAN 0 //Channel 0/1 CE
#define SPEED 3600000 //Bus speed
#define LEN 3 //Length of expected bytes

#define PORT 50141 //Hardcoded default port
#define BUF 16384 //Buff for 1024 Samples each round in shorts
#define SIZESHORT 16

//Method which gets called when on error occured.
void error_func(char *errormsg){
	fprintf(stderr, "%s: %s, \n", errormsg, strerror(errno));
	exit(EXIT_FAILURE);
}

//Helper function which shifts bits to get 10 Bit resolution from adc out of given char array.
unsigned short doDecimal( unsigned char data[] ){
	unsigned short result;
	result = ((data[1] & 0x03)<<8) | data[2];

	return result;
}

//Helper function to convert the unsigned shorts from host-byte-order to network-byte-order to prevent Big&Little endian erros
void convertHostShortToNetShort(unsigned short *givenArray,unsigned short *convertedArray){

	for (int i = 0; i < sizeof(givenArray); i++){
		convertedArray[i] = htons(givenArray[i]);
	}	 
}


void bind_Service(int *sock, unsigned long adress, unsigned short port){
	struct sockaddr_in server;
	memset( &server, 0, sizeof(server) );
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl( adress );
	server.sin_port = htons( port );
	
	if( bind( *sock, (struct sockaddr*)&server, sizeof(server)) < 0 ){
		error_func("Error while binding service to port: ");
	}
}

void send_Data(int *sock, unsigned short *data, int size, char *addr, unsigned short port){
		struct sockaddr_in target;
		struct hostent *h;
		int rc;
		
		h = gethostbyname(addr);
		if(h == NULL){
			error_func("Unkown host: ");
		}
		target.sin_family = h->h_addrtype;
		memcpy ( (char*)&target.sin_addr.s_addr, h->h_addr_list[0], h->h_length );
		target.sin_port = htons( PORT );

		rc = sendto(*sock, data, size, 0, (struct sockaddr *)&target, sizeof (target) );
		if( rc < 0 ){
			error_func("Couldn't send data: ");
		}
}

int main (void){
/**
 * This area initiate the network communication for the application
 * 
*/
	//Request a socket
	printf("Start to etablish network connection, wait...\n");
	int sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock < 0 ){
		error_func( "Error occured while requesting the network socket: " );
	}

	//Bind service to specified port
	bind_Service(&sock, INADDR_ANY, 0);
	printf("Ok.\n");

	//Setup the SPI-Bus on CE0 and init. CLK.     	
	printf( "Setup the SPI interface..\n" );
	if( wiringPiSetup() || wiringPiSPISetup( CHAN, SPEED ) < 0 ){
		error_func("Failed to setup SPI-Bus: ");
	}
	else{
		printf( "Ok.\n" );
	}
	
	//Main loop for gathering the data from the ADC via SPI-Bus.
	digitalWrite( CS, 1 );
	digitalWrite( CS, 0 );
	char *addr = "zanjoo";
	unsigned char data[3];
	unsigned short res[BUF/SIZESHORT];
	unsigned short toSend[BUF/SIZESHORT]; //Space for 256 samples
	while(1){
		/**
		 * The Method wiringPISPIDataRW always rewrite the array data caused by the natural design of SPI.
		 * This is lack of performance. If someone knows a fix for that i would appreciate. 
		 * 
		*/
		for(int i = 0; i < (BUF/SIZESHORT); i++){
			data[0] = 0x01;
			data[1] = 0x80;
			data[2] = 0x00;
			wiringPiSPIDataRW( CS, data, LEN );
			res[i] = doDecimal(data);
		}
		/**
		 * Whats possible here is to add a que and pass it to another thread.
		*/
		convertHostShortToNetShort(res, toSend);
		send_Data(&sock, toSend, sizeof(toSend), addr, PORT);
	}	
}

/*
 *Author: S.P. Nuerenberg
 *
 * Description: A Software to gather Signals from the MCP3008 ADC which got amplified by the LT1638 R-R OpAMp.
 * The gathered audio signals are streamed over the network to another debian based computer via udp.
 * 
 * Thanks to: 
 * G. Henderson for his lib. wiringPi.
 * https://projects.drogon.net/
 * 
 * */