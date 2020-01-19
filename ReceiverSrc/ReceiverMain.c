#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

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
 * Function to bind this service to an specific port.
*/
void bind_Service(int *sock, unsigned long adress, unsigned short port){
	struct sockaddr_in server;
	memset( &server, 0, sizeof(server) );
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl( adress );
	server.sin_port = htons( port );
	
	if( bind( *sock, (struct sockaddr*) &server, sizeof(server)) < 0 ){
		error_func("Error while binding service to port");
	}
}

/**
 * Function to receive the data send by the sender.
*/
void receiveData( int *sock, char *data, int size){
    struct sockaddr_in fromWhere;
    unsigned int len;
    int n;

    len = sizeof( fromWhere );
    n = recvfrom( *sock, data, size, 0, ( struct sockaddr*) &fromWhere, &len);
    if( n < 0 ){
        error_func("Got no Data over Network.");
    }
}

/**
 * Helper function to convert the net-byte-order to the host-byte-order
*/
void reconstructValues(unsigned short *puffer, unsigned short *res){


};

void convertNetShortToHostShort(unsigned short *givenArray, unsigned short *convertedArray){

	for (int i = 0; i < sizeof(givenArray); i++){
		convertedArray[i] = ntohs(givenArray[i]);
	}	 
}

int contains(unsigned short *res){
    for(int i = 0; i < (BUF/SIZESHORT); i++){
        if(res[i] > 1024)
            return -1;
    }
    return 0;
}

int main(void){
    unsigned short *puffer = (unsigned short *) malloc ((BUF/SIZESHORT) * sizeof(unsigned short));
    unsigned short *res = (unsigned short *) malloc ((BUF/SIZESHORT) * sizeof(unsigned short));

    //Create Client network socket
    int sock = socket( AF_INET, SOCK_DGRAM, 0 );    
    if ( sock < 0 ){
        error_func("Couldn't initialize network socket.");
    }
    //Waits for data from anywhere at PORT
    bind_Service( &sock, INADDR_ANY, PORT );

    while (1)
    {   
        memset(res, 0 , ((BUF/SIZESHORT) * sizeof(unsigned short)));
        memset(puffer, 0 , ((BUF/SIZESHORT) * sizeof(unsigned short)));

        receiveData (&sock, puffer, (BUF/SIZESHORT));

        convertNetShortToHostShort(puffer, res);
        if(contains(res) < 0)
            error_func("Something went wrong while transform to hostbyte order.");

        for(int i = 0; i < (BUF/SIZESHORT); i++){
            printf("%u \n", res[i]);
        }

    }
}
