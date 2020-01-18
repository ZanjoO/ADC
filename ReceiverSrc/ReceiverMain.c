#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT 50141
#define BUF 8192

void error_func(char *errormsg){
	fprintf(stderr, "%s: %s, \n", errormsg, strerror(errno));
	exit(EXIT_FAILURE);
}

void bind_Service(int *sock, unsigned long adress, unsigned short port){
	struct sockaddr_in server;
	memset( &server, 0, sizeof(server) );
	server.sin_family = AF_INET6;
	server.sin_addr.s_addr = htonl( adress );
	server.sin_port = htons( port );
	
	if( bind( *sock, (struct sockaddr*) &server, sizeof(server)) < 0 ){
		error_func("Error while binding service to port");
	}
}

void receiveData( int *sock, unsigned short *data, size_t size){
    struct sockaddr_in fromWhere;
    unsigned int len;
    int n;

    len = sizeof( fromWhere );
    n = recvfrom( *sock, data, size, 0, ( struct sockaddr*) &fromWhere, &len);
    if( n < 0 ){
        error_func("Got no Data over Network.");
    }
}

int main(void){
    unsigned short puffer;

    //Create Client network socket
    int sock = socket( AF_INET6, SOCK_DGRAM, 0 );    
    if ( sock < 0 ){
        error_func("Couldn't initialize network socket.");
    }

    //Waits for data from anywhere at PORT
    bind_Service( &sock, INADDR_ANY, PORT );

    unsigned short res = 0;
    while (1)
    {
        //memset( puffer, 0, BUF );
        receiveData (&sock, &puffer, BUF);

        res = ntohs(puffer);
        printf("Retrieved %hi \n", res);
    }
}
