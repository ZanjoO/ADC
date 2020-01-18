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

/**
 * CS = Chip select pin.
 * CHAN = Channel (Pi has 0 or 1)
 * SPEED = The SPI Bus clock
 * LEN = Expected returned bytes of wiringPiSPIDataRW function
 * PORT = The port on which this service get bind and where the receiver have to listen to
 * BUF = A buffer for 724 samples of type unsigned short.
 * SIZESHORT = size if type short in bit
*/
#define CS 10
#define CHAN 0
#define SPEED 3600000
#define LEN 3
#define PORT 50141
#define BUF 11584 
#define SIZESHORT 16

/**
 * Std-method which gets a call when an error occured and exits the program.
*/
void error_func(char *errormsg){
	fprintf(stderr, "%s: %s, \n", errormsg, strerror(errno));
	exit(EXIT_FAILURE);
}

/**
 * Helper function which shifts bits to get 10 Bit resolution from adc out of given char array. 
*/
unsigned short doDecimal( unsigned char data[] ){
	unsigned short result;
	result = ((data[1] & 0x03)<<8) | data[2];

	return result;
}

/**
 * Helper function which converts a array of short in host-byte-order to network-byte-order
*/
void convertHostShortToNetShort(unsigned short *givenArray,unsigned short *convertedArray){

	for (int i = 0; i < sizeof(givenArray); i++){
		convertedArray[i] = htons(givenArray[i]);
	}	 
}

/**
 * Function to bind this service to an specific port.
*/
void bind_Service(int *sock, unsigned short port){
	struct sockaddr_in server;
	memset( &server, 0, sizeof( server ));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl( INADDR_ANY );
	server.sin_port = htons( port );

	if( bind( *sock, ( struct sockaddr * )&server, sizeof( server )) < 0 ){
		error_func("Error while binding service to port: ");
	}
}

/**
 * Function to send the gathered data via udp to port.
*/
void send_Data(int *sock, unsigned short *data, int size, char *addr, unsigned short port){
		struct sockaddr_in target;
		struct hostent *h;
		int rc;
		
		h = gethostbyname(addr); //Deprecated better use getaddrinfo(); 
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
 * Area where the network gets initialized.
*/
	printf("Start to etablish network connection, wait...\n");
	int sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock < 0 ){
		error_func( "Error occured while requesting the network socket: " );
	}

	bind_Service(&sock, 0);
	printf("Ok.\n");

/**
 * Initializing the SPI-Bus.
*/    	
	printf( "Setup the SPI interface..\n" );
	if( wiringPiSetup() || wiringPiSPISetup( CHAN, SPEED ) < 0 ){
		error_func("Failed to setup SPI-Bus: ");
	}
	else{
		printf( "Ok.\n" );
	}
	
/**
 * This is the programs main loop area to gather data and send'em via network.
*/

/**
 * As given in the datasheet of the MCP3008 i toogle to initiate communication.
*/
	digitalWrite( CS, 1 );
	digitalWrite( CS, 0 );
	char *addr = "zanjoo";
	unsigned char data[3];
	unsigned short res[BUF/SIZESHORT];
	unsigned short toSend[BUF/SIZESHORT];

	while(1){

		for(int i = 0; i < (BUF/SIZESHORT); i++){
			//memset(res, 0 , (BUF/SIZESHORT));
			data[0] = 0x01;
			data[1] = 0x80;
			data[2] = 0x00;
			wiringPiSPIDataRW( CS, data, LEN );
			res[i] = doDecimal(data);
		}
		//convertHostShortToNetShort(res, toSend); Both systems work with little endian only caused trouble with garbage values
		
		for(int i = 0; i < sizeof(res); i++){
			printf("%p\n", (void *)&res[i]);
		}

		send_Data(&sock, toSend, sizeof(toSend), addr, PORT);

	}	
}

/*
 *Author: S.P. Nürenberg
 *
 * Description: A Software to gather Signals from the MCP3008 ADC which got amplified by the LT1638 R-R OpAMp.
 * The gathered audio signals are streamed over the network to another debian based computer via udp.
 * 
 * Thanks to: 
 * G. Henderson for his lib. wiringPi.
 * https://projects.drogon.net/
 * 
*/