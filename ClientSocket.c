#include <sys/types.h>
#include <sys/socket.h>

/**
 * Verwendete Protokollfamilie
 * @param domain Domainfamily
 * 
 * Definiert den Typ des Protokols.
 * @param type
 * 
 * @param protocol
*/
int socket(int domain, int type, int protocol);

int main (void){


/**
 * Gibt den Filedeskriptor zurück welcher <0 einen error der Socketinitialisierung angibt
 * @return Linux file-descriptor
*/
int communicationSock = socker(AF_INET, SOCK_DGRAM , 0);
if (communicationSock < 0)







}