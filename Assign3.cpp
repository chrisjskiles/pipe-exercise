/***************************************************************************************
NAME: Chris Skiles
ZID : z1638506
ASGN: 3
USE : Demonstrate pipe() and fork() system calls.
****************************************************************************************/
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>

using namespace std;

void PWork(int, int);
void CWork(int, int);
void GWork(int, int);

int main() {
	
	//arrays to receive pipe fds
	int pipeA[2],
		pipeB[2],
		pipeC[2];
		
	int status,		//receives return code of pipe so we know if the call was successful
		pid;
	
	//open 3 pipes and check to make sure each was successful
	status = pipe(pipeA);
	if (status == -1) {
		cerr << "pipe error";
		exit(-5);
	}
	
	status = pipe(pipeB);
	if (status == -1) {
		cerr << "pipe error";
		exit(-5);
	}
	
	status = pipe(pipeC);
	if (status == -1) {
		cerr << "pipe error";
		exit(-5);
	}
	
	//first fork
	pid = fork();
	
	//exit on error
	if (pid == -1) {
		cerr << "fork error";
		exit(-5);
	}
	
	else if (pid == 0) { //child process
	
		//2nd fork
		pid = fork();
		
		//exit on error
		if (pid == -1) {
			cerr << "fork error";
			exit(-5);
		}
		
		else if (pid == 0) { //grandchild
		
			//close unused pipe ends and call helper function
			close(pipeB[1]);
			close(pipeC[0]);
			GWork(pipeB[0], pipeC[1]);
			
			exit(0);
		}
		
		else { //still child
		
			//close unused pipe ends and call helper function
			close(pipeA[1]);
			close(pipeB[0]);
			CWork(pipeA[0], pipeB[1]);
			
			wait(0);
			exit(0);
		}
	}
	
	else { //parent process
	
		//close unused pipe ends and call helper function
		close(pipeC[1]);
		close(pipeA[0]);
		PWork(pipeC[0], pipeA[1]);
		
		wait(0);
		exit(0);
	}
	
	return 0;
}

//does work for parent process
void PWork(int readEnd, int writeEnd) {
	char InBuffer[50], 
	     OutBuffer[50],
	     messageLength[1],
	     MString[20];
 
	char *token;	//for use with strtok
 
	string message;
		 
	int L,			//length of received message
        M = 1, 		//int passed between processes
	    Counter = 0;
	
	//initial message for pipe
	message = "Message 0 from Parent:     Value = 1";
	strcpy(OutBuffer, message.c_str());
	
	//write initial message to the pipe
	write(writeEnd, to_string(strlen(OutBuffer)).c_str(), 2);
	write(writeEnd, OutBuffer, strlen(OutBuffer));

	while (Counter <= 8) {
		//read message length from pipe
		read(readEnd, messageLength, 2);
		L = atoi(messageLength);

		//read message from pipe and append a null terminator
		read(readEnd, InBuffer, L);
		InBuffer[L] = '\0';
		
		//output message
		cerr << InBuffer << endl;
		
		//tokenize string; the last token contains the M
		token = strtok(InBuffer, " ");
		while (token != NULL) {
			strcpy(MString, token);
			token = strtok(NULL, " ");
		}
		
		//compute new M and increment counter
		M = atoi(MString);
		M = 2 * M + 3;
		
		++Counter;
		
		//build next message
		message = string("Message " )+ to_string(Counter) + " from Parent:     Value = " + to_string(M);
		strcpy(OutBuffer, message.c_str());
		
		//write message to pipe
		write(writeEnd, to_string(strlen(OutBuffer)).c_str(), 2);
		write(writeEnd, OutBuffer, strlen(OutBuffer));
	}
}

//does work for child process
void CWork(int readEnd, int writeEnd) {
	char InBuffer[50], 
         OutBuffer[50],
	     messageLength[1],
	     MString[20];
		 
	char *token;	//for use with strtok
		 
	string message;
		 
	int L,			//length of received message
		M = 1, 		//int passed between processes
		Counter = 0;
	
	while (Counter <= 8) {
		//read message length from pipe
		read(readEnd, messageLength, 2);
		L = atoi(messageLength);
		
		//read message from pipe and append a null terminator
		read(readEnd, InBuffer, L);
		InBuffer[L] = '\0';
		
		//output message
		cerr << InBuffer << endl;
		
		//tokenize string; the last token contains the M
		token = strtok(InBuffer, " ");
		while (token != NULL) {
			strcpy(MString, token);
			token = strtok(NULL, " ");
		}
		
		//computer new M and increment counter
		M = atoi(MString);
		M = 2 * M + 3;
		
		++Counter;
		
		//build next message
		message = string("Message " )+ to_string(Counter) + " from Child:      Value = " + to_string(M);
		strcpy(OutBuffer, message.c_str());
		
		//write message to pipe
		write(writeEnd, to_string(strlen(OutBuffer)).c_str(), 2);
		write(writeEnd, OutBuffer, strlen(OutBuffer));
	}
}

//does the work for the grandchild process
void GWork(int readEnd, int writeEnd) {
	char InBuffer[50], 
	     OutBuffer[50],
             messageLength[1],
	     MString[20];
		 
	char *token;	//for use with strtok
		 
	string message;
		 
	int L,			//length of received message
		M = 1, 		//int passed between processes
		Counter = 0;
	
	while (Counter <= 8) {
		//read message length from pipe
		read(readEnd, messageLength, 2);
		L = atoi(messageLength);
		
		//read message from pipe and append a null terminator
		read(readEnd, InBuffer, L);
		InBuffer[L] = '\0';
		
		//output message
		cerr << InBuffer << endl;
		
		//tokenize string; the last token contains the M
		token = strtok(InBuffer, " ");
		while (token != NULL) {
			strcpy(MString, token);
			token = strtok(NULL, " ");
		}
		
		//computer new M and increment counter
		M = atoi(MString);
		M = 2 * M + 3;
		
		++Counter;
		
		//build next message
		message = string("Message " )+ to_string(Counter) + " from Grandchild: Value = " + to_string(M);
		strcpy(OutBuffer, message.c_str());
		
		//write message to pipe
		write(writeEnd, to_string(strlen(OutBuffer)).c_str(), 2);
		write(writeEnd, OutBuffer, strlen(OutBuffer));
	}
}
