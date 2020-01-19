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
#define GPIPEG "gnuplot -presistent"

/**
 * Std-method which gets a call when an error occured and exits the program.
*/
void 
error_func(char *errormsg)
{
	fprintf(stderr, "%s: %s, \n", errormsg, strerror(errno));
	exit(EXIT_FAILURE);
}

/**
 * Function to bind this service to an specific port.
*/
void 
bind_Service(int *sock, unsigned long adress, unsigned short port)
{
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
void 
receiveData( int *sock, unsigned short data[], int size)
{
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
 * Helper function to convert the net-byte-order to the host-byte-order.
*/
void 
convertNetShortToHostShort(unsigned short givenArray[], unsigned short convertedArray[])
{

	for (int i = 0; i < (BUF/SIZESHORT); i++){
		convertedArray[i] += ntohs(givenArray[i]);
	}	 
}

/**
 * Main from Receiver, receives pakets and plots data with tool gnuplot.
*/
int main(void)
{

/**
 * Initialization of gnuplot.
 * Pipe commands for style to gnuplot after init.
*/
    const char* name = "Audiosignals";
    FILE *gnuPipe = popen(GPIPEG, "w");
    if (gnuPipe < 0)
    {
        error_func("Failed to initialize the GNUPIPE.");
    }

    fprintf(gnuPipe, "set title '%s'\n", name);
    fprintf(gnuPipe, "set xlabel \"Test\"");


/**
 * Initialisation of the network "communication" for receiver.
*/
    int sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if ( sock < 0 ){
        error_func("Couldn't initialize network socket.");
    }
    bind_Service( &sock, INADDR_ANY, PORT );

    unsigned short puffer[BUF/SIZESHORT];
    unsigned short res[BUF/SIZESHORT];

    while (1)
    {   
        memset(res, 0 , ((BUF/SIZESHORT) * sizeof(unsigned short)));
        memset(puffer, 0 , ((BUF/SIZESHORT) * sizeof(unsigned short)));

        receiveData (&sock, puffer, ((BUF/SIZESHORT) * sizeof(unsigned short)));
        convertNetShortToHostShort(puffer, res);
        
        for(int i = 0; i < (BUF/SIZESHORT); i++){
            fprintf(gnuPipe, "%u \n", res[i]);
        }
        fprintf(gnuPipe, "e\n");
        fprintf(gnuPipe, "refresh\n");
    }
}
