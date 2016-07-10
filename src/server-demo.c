#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h>
#include "Sha256Calc.h"

#define MAXDATASIZE 1024

#define SERVERPORT 8000
#define MAXCONN_NUM 10

static int rand_data;
static int key_file[32] = {0,1,2,3,4,5,6,7};

int main( )
{ 
    char *rule = "Hello";
    char buf[ MAXDATASIZE] ; 
    int sockfd, new_fd, numbytes; 
    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr; 
    int sin_size; 
    char enc_data[256] = {0};
    Sha256Calc  shac;
    char *enp = "happylife1k";

    rand_data = rand();

    if ( ( sockfd = socket ( AF_INET , SOCK_STREAM , 0) ) == -1) { 
        perror ( "socket error" ) ; 
        return 1; 
    } 
    memset ( & client_addr, 0, sizeof ( struct sockaddr ) ) ; 
    server_addr.sin_family = AF_INET ; 
    server_addr.sin_port = htons ( SERVERPORT) ; 
    server_addr.sin_addr.s_addr = INADDR_ANY ; 
    if ( bind ( sockfd, ( struct sockaddr * ) & server_addr, sizeof ( struct sockaddr ) ) == -1) { 
        perror ( "bind error" ) ; 
        return 1; 
    } 
    if ( listen ( sockfd, MAXCONN_NUM) == -1) { 
        perror ( "listen error" ) ; 
        return 1; 
    } 

    while ( 1) { 
        sin_size = sizeof ( struct sockaddr_in ) ; 
        if ( ( new_fd = accept ( sockfd, ( struct sockaddr * ) & client_addr, & sin_size) ) == -1) { 
            perror ( "accept error" ) ; 
            continue ; 
        } 
        printf ( "server: got connection from %s/n" , inet_ntoa( client_addr. sin_addr) ) ; 
        if ( ( numbytes = recv ( new_fd, buf, MAXDATASIZE, 0) ) == -1) { 
            perror ( "recv error" ) ; 
            return 1; 
        }

        Sha256Calc_reset(&shac);
        Sha256Calc_calculate( &shac, (SZ_CHAR*) enp, strlen(enp) );
 
        if ( numbytes) { 
            buf[ numbytes+1] = '\0'; 

            if (memcmp(buf, &shac.Value, 8) == 0 ) {
                printf ( " Authentication success. received: %s.\n" , buf) ;
            } else {
                printf ( "buf:%s  rule:%s\n", buf, rule) ;
                printf ( "Invalid data Authentication failed!\n") ;
                close (new_fd);
            }


            sleep ( 3) ; 
        } 
        if ( send ( new_fd, "hi~~" , 5, 0) == -1) { 
            perror ( "send error" ) ; 
            return 1; 
        } 
		if (new_fd) {
			close(new_fd);
		}
    } 
    return 0; 
}

