#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/select.h>

#define PORT 44444

int main()
{       
    int descriptor;									// socket file descriptor
    ssize_t receive;
    char buffer [1500];				
    struct sockaddr_in server;		
    socklen_t len = sizeof(server);
    
    descriptor = socket(AF_INET, SOCK_DGRAM, 0);    //parameters for UDP
    if(descriptor == -1)
            perror("Error socket()");
    else
            printf("Descriptor: %i\n", descriptor);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);                              // set prefered Port  
    
    bind(descriptor,(struct sockaddr*) &server, len);	        // bind server to address

    fd_set rfds;
    struct timeval tv;
    int retval;   
    while(1)
    {               
    	FD_ZERO(&rfds);
    	FD_SET(descriptor, &rfds);
		tv.tv_sec = 5;
		tv.tv_usec = 0;
        
        retval = select(descriptor+1, &rfds, NULL, NULL, &tv);
        if(retval < 0)
            perror("Error select()\n");
        else if(retval > 0)
        {
            receive = recvfrom(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &server, &len);
	    	if(receive < 0)
	        	perror("Error receive()\n");
		
			printf("%s\n", buffer);								// print message
			sendto(descriptor, buffer, sizeof(buffer), 0,(struct sockaddr*) &server, len);		// sends received message back, maybe use as ACK
         }    
    }

    return EXIT_SUCCESS;
}