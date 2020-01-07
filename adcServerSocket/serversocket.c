#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/type.h>


//Syntax variiert von OS zu OS
int socket(int domain, int type int protocol);


int main(){

	//Erhalt und überprüfen des socket deskriptors
	//IpV4, UDP, Standard UDP
	int sock = socket ( AF_NET, SOCK_STREAM, 0 );
	if ( sock < 0){
		printf( "Fehler bei Socketanforderung" );
		exit(1);
	}
	





}
