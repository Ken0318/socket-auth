#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include "sha256calc.h"

#define MAXDATASIZE 1024

#define SERVERIP "192.168.7.2" 
#define SERVERPORT 8000
#define AUTH_FILE "/home/auth.key"

int main(int argc, char **argv) 
{ 
    char buf[MAXDATASIZE] ; 
    int sockfd, numbytes; 
    struct sockaddr_in server_addr; 
    Sha256Calc  shac;
    char enc_data[256] = {0};
 
    FILE *fp;
    fp = fopen(AUTH_FILE, "r");
    if (fp == NULL)
        return 1;
    while(fgets(enc_data, 255, fp) != NULL) {}

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0) ) == -1) { 
        perror("socket error") ; 
        return 1; 
    } 

    memset(&server_addr, 0, sizeof(struct sockaddr)) ; 
    server_addr.sin_family = AF_INET ; 
    server_addr.sin_port = htons ( SERVERPORT) ; 
    server_addr.sin_addr.s_addr = inet_addr( SERVERIP) ; 
    if (connect (sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) { 
        perror("connect error") ; 
        return 1; 
    } 

    Sha256Calc_reset(&shac);
    Sha256Calc_calculate(&shac, (SZ_CHAR*) enc_data, strlen(enc_data));
    if (send(sockfd, &shac.Value, strlen(enc_data), 0) == -1) {
        perror ( "send error" ) ;
        return 1;
    }

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) { 
        perror ( "recv error" ) ; 
        return 1; 
    } 

    if (numbytes) { 
        buf[ numbytes] = '\0' ; 
    } 

    close(sockfd) ; 
    return 0; 
}
