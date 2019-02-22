//Gabriel Equitz - CSC645 - SFSU ID: 915254839
//
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */


#define MX_LENGTH 100 /*maximum length for user name, password */
#define MX_MSG_LENGTH 500 /* maximum length for message */
#define MAX_CONNECTIONS 4 //maximum number of connections

#define RCVBUFSIZE 256 /* Size of receive buffer */

void DieWithError(char *errorMessage); /* Error handling function */

int main(int argc, char *argv[])
{
    int sock, chatSock, clntLen, clntSock; 
    struct sockaddr_in echoServAddr; 
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort; 
    char password[MX_LENGTH], receiver[MX_LENGTH] ;
    char message[MX_MSG_LENGTH], message1[MX_MSG_LENGTH], sender[MX_LENGTH]; 
    char server_IP[MX_LENGTH], user_name[MX_LENGTH], user_name2[MX_LENGTH] ;
    char *echoString; 
    char echoBuffer[RCVBUFSIZE], choice[1]; 
    int connected = 0, bytesRcvd, totalBytesRcvd, recvMessageSize; 
    unsigned int echoStringLen;


    if (argc == 4)
        echoServPort = atoi(argv[3]); /* Use given port, if any */
    else
        echoServPort = 7; /* 7 is the well-known port for the echo service */

/* Receive the same string back from the server */
    totalBytesRcvd = 0;
    for(;;)
    {

        /* Receive up to the buffer size (minus i to leave space for a null terminator) bytes from the sender */
	printf("0. Connect to the server\n");
	printf("1. Get the user list\n");
	printf("2. Send a message\n");
	printf("3. Get my messages\n");
	printf("4. Start a chat with my friend\n");
	printf("5. Chat with my friend\n");
	printf("Your option (enter a number): ");
	scanf("%s", &choice);
        

	/* send the string to the server */
	echoStringLen = strlen(choice);
	if(connected == echoStringLen){
	    if(send(sock, choice, echoStringLen, 0) != echoStringLen){
	        DieWithError("send function sent a different number of bytes than expected!");}
	}

	//start the connection if choice is 0
	if(strcmp(choice, "0")==0){
	    if(connected == 0){
	    	printf("Please enter the IP address: ");
		scanf("%s", &server_IP);
	    	printf("Please enter the port number: ");
	    	scanf("%d", &echoServPort);
	    	printf("Connecting...\n");

	    	// Create a stream socket using TCP 
    	    	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		    DieWithError(" socket function failed") ;}

	        memset(&echoServAddr, 0, sizeof(echoServAddr)); //constructing the server address

	        echoServAddr.sin_family = AF_INET; //zero out structure

 	        echoServAddr.sin_addr.s_addr = inet_addr(server_IP); //family of internet address

      	        echoServAddr.sin_port = htons(echoServPort);  //server port

	        //establishing connection
    	        if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
	     	   DieWithError(" connect function failed");}
		connected = 1;
	    }
	    if(send(sock, choice, 1, 0) != 1){
	        DieWithError("send function sent a different number of bytes than expected!");}
	    printf("Connected!\nWelcome! Please log in.\nUsername: ");
	    scanf("%s", &user_name);
	    echoStringLen = strlen(user_name);
	    if(send(sock, user_name, echoStringLen, 0) != echoStringLen){
	        DieWithError("send function sent a different number of bytes than expected!");}

	    printf("Password: ");
	    scanf("%s", &password);
	    echoStringLen = strlen(password);
	    if(send(sock, password, echoStringLen, 0) != echoStringLen){
	        DieWithError("send function sent a different number of bytes than expected!");}

	    if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, 0)) < 0){
            	DieWithError("recv function failed");}
	    printf("%s\n", echoBuffer);
	    if(strcmp(echoBuffer, "login fail\n")==0){
		exit(0);}

	} 
	
	else if(strcmp(choice, "1")==0){
	    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
                DieWithError("recv function failed or connection closed prematurely");}
	    echoBuffer[bytesRcvd] = '\0';
	    printf("%s\n", echoBuffer);

	} 
	else if(strcmp(choice, "2")==0){
	    printf("Please enter the username: ");
	    scanf("%s", receiver);
	    echoStringLen = strlen(receiver);

	    if(send(sock, receiver, echoStringLen, 0) != echoStringLen){
	        DieWithError("send function sent a different number of bytes than expected!");}

	    printf("Please enter the message: ");
	    scanf("%s", message);
	    fgets(message1, MX_MSG_LENGTH, stdin);
	    strcat(message, message1);
	    echoStringLen = strlen(message);

	    if(send(sock, message, echoStringLen, 0) != echoStringLen){
	        DieWithError("send function sent a different number of bytes than expected!");}

	    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
                DieWithError("recv function failed or connection closed prematurely");}

	    echoBuffer[bytesRcvd] = '\0';
	    printf("%s\n", echoBuffer);

	} else if(strcmp(choice, "3")==0){

	    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
                DieWithError("recv function failed or connection closed prematurely");}

	    echoBuffer[bytesRcvd] = '\0';
	    printf("%s\n", echoBuffer);

	} else if(strcmp(choice, "4")==0){
	    close(sock);
	    printf("Please enter the port number you want to listen on: ");
	    scanf("%d", &echoServPort);

	    if ((chatSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
	        DieWithError( "socket function failed");}

	    echoServAddr.sin_port = htons(echoServPort);

	    if (bind(chatSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0){
        	DieWithError ( "bind function failed");}

	    //marking the socket to listen to the incoming connection 
    	    if (listen(chatSock, MAX_CONNECTIONS) < 0){
	        DieWithError("listen function failed") ;}

	    printf("I am listening on %s:%d!\n", server_IP, echoServPort);
	    clntLen = sizeof(echoClntAddr); //setting size of in-out parameter

            //waiting for the client to connect
            if ((clntSock = accept(chatSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0){
                DieWithError("accept function failed");}
	    printf("Client connected!\n");

	    //cannot connect
	    if ((recvMessageSize = recv(clntSock, user_name2, MX_LENGTH, 0)) < 0){
                DieWithError("recv function failed");}
	    user_name2[recvMessageSize] = '\0';
	    printf("%s is listening.\n", user_name2);
	    echoStringLen = strlen(user_name);

	    if(send(clntSock, user_name, echoStringLen, 0) != echoStringLen){
		    DieWithError("send function sent a different number of bytes than expected!");}

	    // run forever
	    for(;;){

	    	if ((recvMessageSize = recv(clntSock, message, MX_MSG_LENGTH, 0)) < 0){
                    DieWithError("recv function failed");}
	        message[recvMessageSize] = '\0';
		printf("Type 'Bye' to stop the conversation!\n");
		printf("%s: %s", user_name2, message);

		if(strcmp(message, "Bye\n")==0)
		{ 
			break;
		}
		memset(message, 0, MX_MSG_LENGTH);
		printf("%s: ", user_name);
		scanf("%s", message);
	    	fgets(message1, MX_MSG_LENGTH, stdin);
		strcat(message, message1);
		echoStringLen = strlen(message);

		if(send(clntSock, message, echoStringLen, 0) != echoStringLen){
		    DieWithError("send function sent a different number of bytes than expected!");}

		if(strcmp(message, "Bye\n")==0) 
		{
			break;
		}}
	    close(chatSock);

	}
       	else if(strcmp(choice, "5")==0){

	    close(sock);
	    printf("Please enter your friend's IP address: ");
	    scanf("%s", &server_IP);
	    printf("Please enter your friend's port number: ");
	    scanf("%d", &echoServPort);
	    printf("Connecting to your friend...\n");
	    memset(&echoServAddr, 0, sizeof(echoServAddr)); //server address structure

        if ((chatSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		DieWithError(" socket function failed") ;}
	    
	    echoServAddr.sin_family = AF_INET;
	    echoServAddr.sin_addr.s_addr = inet_addr(server_IP);
	    
      	    echoServAddr.sin_port = htons(echoServPort); //server port
	    

        if (connect(chatSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
	        DieWithError(" connect function failed");}
	    printf("Connected!\n");
	    echoStringLen = strlen(user_name);

	    if(send(chatSock, user_name, echoStringLen, 0) != echoStringLen){
		DieWithError("send function sent a different number of bytes than expected.");}

	    if ((recvMessageSize = recv(chatSock, user_name2, MX_MSG_LENGTH, 0)) < 0){
                    DieWithError("recv function failed");}

	    for(;;){

		printf("Type 'Bye' to stop the conversation.\n");
		printf("%s: ", user_name);
		scanf("%s", message);
	    	fgets(message1, MX_MSG_LENGTH, stdin);
		strcat(message, message1);
		echoStringLen = strlen(message);

		if(send(chatSock, message, echoStringLen, 0) != echoStringLen){
		    DieWithError("send function sent a different number of bytes than expected.");}

		if(strcmp(message, "Bye\n")==0){ 
			break;
		}

	    	if ((recvMessageSize = recv(chatSock, message, MX_MSG_LENGTH, 0)) < 0){
                    DieWithError("recv function failed");}
	        message[recvMessageSize] = '\0';
		printf("%s: %s", user_name2, message);

		if(strcmp(message, "Bye\n")==0){ 
			break;
		}

		memset(message, 0, MX_MSG_LENGTH);}
	    close(chatSock);
	} 
	else {
		exit(0);
	}
	printf("------------------\n");
    }

    printf("\n"); 

    close(sock);
    exit(0);
}

