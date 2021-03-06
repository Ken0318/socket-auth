#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <stdlib.h>
#include "sha256calc.h"

#define MAXDATASIZE 1024

#define SERVERPORT 8000
#define MAXCONN_NUM 10
#define AUTH_FILE "/home/auth.key"

void doprocessing (int sock) 
{
   int n;
   char buffer[256];

   while (1) {
   	   bzero(buffer,256);
       n = read(sock,buffer,255);
   
       if (n < 0) {
          perror("ERROR reading from socket");
          exit(1);
       }
   
       printf("Here is the message from client: %s\n",buffer);
       n = write(sock,"I got your message",18);
   
       if (n < 0) {
          perror("ERROR writing to socket");
          exit(1);
       }
    }
	
}


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
    FILE *fp;
	int pid;

    fp = fopen(AUTH_FILE, "r");
    if (fp == NULL)
        return 1;
    while (fgets(enc_data, 255, fp) != NULL) {}

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket error") ; 
        return 1; 
    } 

    memset(&client_addr, 0, sizeof(struct sockaddr)); 
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(SERVERPORT) ; 
    server_addr.sin_addr.s_addr = INADDR_ANY ; 
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) { 
        perror ( "bind error" ) ; 
        return 1; 
    } 
    if (listen(sockfd, MAXCONN_NUM) == -1) { 
        perror("listen error" ) ; 
        return 1; 
    } 

    while (1) { 
        sin_size = sizeof(struct sockaddr_in) ; 
        if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) { 
            perror("accept error") ; 
            continue ; 
        }

        printf("server: got connection from %s/n", inet_ntoa(client_addr.sin_addr)); 
        if ((numbytes = recv(new_fd, buf, MAXDATASIZE, 0)) == -1) { 
            perror ( "recv error" ) ; 
            return 1; 
        }

        Sha256Calc_reset(&shac);
        Sha256Calc_calculate(&shac, (SZ_CHAR*)enc_data, strlen(enc_data));
 
        if (numbytes) { 
            buf[numbytes+1] = '\0'; 

            if (memcmp(buf, &shac.Value, 8) == 0 ) {
                printf("Authentication success. received: %s.\n" , buf) ;
            } else {
                printf("buf:%s  rule:%s\n", buf, rule) ;
                printf("Invalid data Authentication failed!\n") ;
                close(new_fd);
            }

        }

		pid = fork();
		  
		if (pid < 0) {
		   perror("ERROR on fork");
		   exit(1);
		}
		
		if (pid == 0) {
		   /* This is the client process */
		   doprocessing(new_fd);
		   exit(0);
		} else {
		   close(new_fd);
		}

    } 

    return 0; 
}

