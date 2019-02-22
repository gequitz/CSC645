//Gabriel Equitz - CSC645 - SFSU ID: 915254839

#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 256
#define MX_LENGTH 100 // max length for user name and password
#define MX_MSG_LENGTH 500
#define MX_NUMBER_USER 2
#define MX_USER_LENGTH_NAME 25
#define MX_BUFFER_SIZE 10
#define MAX_CONNECTIONS 4 //maximum number of connections

void DieWithError(char *errorMessage); /* Error handling function */

int main(int argc, char *argv[])
{

    int servSock; /* Socket descriptor for server */
    int clntSock; /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort; /* Server port */
    unsigned int clntLen; /* Length of client address data structure */


    char  password_id[MX_NUMBER_USER][MX_USER_LENGTH_NAME], choice[MX_NUMBER_USER]; 
    char sender[MX_LENGTH], message[MX_LENGTH], messageStat[MX_LENGTH] ;
    char echoBuffer[RCVBUFSIZE], size_Buffer[MX_BUFFER_SIZE];
    int client_number = 0, recvMessageSize = 1, login = 0, i, echoStrLen;
    char user_name[MX_LENGTH], password[MX_LENGTH], receiver[MX_LENGTH]; 
    char user_id[MX_NUMBER_USER][MX_USER_LENGTH_NAME] ;
    int message_number = 0;

    echoServPort = 8000;  //local port

    //store user name and password
    strcpy(user_id[0], "Alice");
    strcpy(user_id[1], "Bob");
    strcpy(password_id[0], "12345");
    strcpy(password_id[1], "56789");
   


    // Creating socket for incoming connections 
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError( "socket function failed") ;

    // Constructinng local address 
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort); //local port

    //binding to local address
    if (bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError ( "bind function failed");

    //Marking the socket to listen to incoming connections
    if (listen(servSock, MAX_CONNECTIONS) < 0)
        DieWithError("listen function failed") ;

    printf("Server has been started!\n");
    printf("Available on 127.0.0.1:%d\n", echoServPort);
    for (;;) /* Run forever */
    {
        //setting the size of the in-out parameter
        clntLen = sizeof(echoClntAddr);

	//waiting for a client to connect 
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept function failed");

	//socket connected to a client
	client_number++;
        printf("The client %d is connected.\n", client_number);

	if ((recvMessageSize = recv(clntSock, choice, 2, 0)) < 0)
            DieWithError("recv function failed");
	choice[recvMessageSize] = '\0';
	
	//send received string
    	while (recvMessageSize > 0)  //zero is the end of the tranmission
    	{

        //Echo message back to client 
	    memset(echoBuffer, 0, RCVBUFSIZE);

	    if(login ==1){
		if ((recvMessageSize = recv(clntSock, choice, 2, 0)) < 0){
            	    DieWithError("recv function failed");}}

	    if(strcmp(choice, "0")==0){
	        if ((recvMessageSize = recv(clntSock, user_name, RCVBUFSIZE, 0)) < 0)
            	    DieWithError("recv function failed: unknown user") ;
		printf("Login user_name is %s\n", user_name);

		if ((recvMessageSize = recv(clntSock, password, RCVBUFSIZE, 0)) < 0)
            	    DieWithError("recv function failed: wrong password");
		printf("Login password is %s\n", password);
		strcpy(echoBuffer, "login fail\n");

		for(i = 0; i < 2; i++){
		    if(strcmp(user_name, user_id[i])==0){
			if(strcmp(password, password_id[i]) == 0){
			    strcpy(echoBuffer, "login OK\n");
			    login=1;}}}

		printf("%s", echoBuffer);
		echoStrLen = strlen(echoBuffer);
		if (send(clntSock, echoBuffer, echoStrLen, 0) != echoStrLen){
	            DieWithError("send function failed: Login Status");}
	    }

	    else if(strcmp(choice, "1")==0){
		snprintf(size_Buffer, MX_BUFFER_SIZE, "%d", i);
		strcpy(echoBuffer, "There are ");
		strcat(echoBuffer, size_Buffer);
		strcat(echoBuffer, " user_id:\n");

		for(i=0; i< 2; i++){
		    strcat(echoBuffer, user_id[i]);
		    strcat(echoBuffer, "\n");}
		echoStrLen = strlen(echoBuffer);

		if(send(clntSock, echoBuffer, echoStrLen, 0) != echoStrLen)
	            DieWithError("send function failed");
		printf("Return User List!\n");
		
	    } else if(strcmp(choice, "2")==0){

	        if ((recvMessageSize = recv(clntSock, receiver, RCVBUFSIZE, 0)) < 0){
            	    DieWithError("recv function failed: receiver not found");}
		strcpy(sender, user_name);

		if ((recvMessageSize = recv(clntSock, message, RCVBUFSIZE, 0)) < 0){
            	    DieWithError("recv function failed: message cannot be verified");}
		message[recvMessageSize] = '\0';
		printf("A message to %s\n", receiver);	
		printf("Message is %s\n", message);
		strcpy(messageStat, "Message has been delivered successfully.");
		echoStrLen = strlen(messageStat);
		messageStat[strlen(messageStat)] = '\0';

		if(send(clntSock, messageStat, strlen(messageStat), 0) != strlen(messageStat))
	            DieWithError("send function failed");
		memset(messageStat, 0, MX_LENGTH);

	    } else if(strcmp(choice, "3")==0){

	    	strcpy(echoBuffer, "You have ");

		if(strcmp(receiver, user_name)==0){
		    message_number+=1;
		    char numb[2];
		    snprintf(numb, 2, "%d", message_number);
		    strcat(echoBuffer, numb);
		    strcat(echoBuffer, " messages.\n");
		    strcat(echoBuffer, message);
		    memset(message, 0, MX_MSG_LENGTH);

		} else {
		    strcat(echoBuffer, "0 message.\n");
		}

		echoStrLen = strlen(echoBuffer);
		echoBuffer[echoStrLen] = '\0';

		if(send(clntSock, echoBuffer, echoStrLen, 0) != echoStrLen){
	            DieWithError("send function failed");}

		message_number = 0;
		printf("Sent back %s's message.\n", receiver);
	    }
        }
        printf("Client has been disconnected.\n");
        close(clntSock); 
	memset(user_name, 0, MX_LENGTH);
	memset(password, 0, MX_LENGTH);

	login = 0;
    }
}


