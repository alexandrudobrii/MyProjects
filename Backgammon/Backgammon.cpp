#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>

#include "BGBoard.h"

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void sendMessage(int sockfd, char* message, int length) {
	int sent_so_far = 0, ret = 0;
	while (sent_so_far < length) {
		ret = send(sockfd, message + sent_so_far, length - sent_so_far, 0);
		if (ret == -1) {
		  error("Can't send message");
			return;
		}
		sent_so_far += ret;
	}
}

void readMessage(int sockfd, char* message, int length) {
	int read_so_far = 0, ret = 0;
	while (read_so_far < length) {
		ret = recv(sockfd, message + read_so_far, length - read_so_far, 0);
		if (ret == -1) {
		  error("Can't read message");
			return;
		}
		read_so_far += ret;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 5) {
		printf("Usage: %s server_hostname server_port opponent_level"
		    "(1=dumb, 5, 7, 8)own_level(1=dumb, 5, 7, 8)\n", argv[0]);
		exit(0);
	}

	//Datele pentru conexiune
	int sockfd, portno;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//Initializari & creare socket
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		error("ERROR opening socket");
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);

	//Conectare la server
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}

	//Trimitem primul mesaj - dificultatea adversarului
	char buffer[256];
	bzero(buffer,256);
	buffer[0] = atoi(argv[3]);
	char message_size = 1; // Trimitem 1 octet
	sendMessage(sockfd, &message_size, 1);
	sendMessage(sockfd, buffer, 1);


	// primesc raspuns cu culoarea
	Player player;
	bzero(buffer,256);
	char read_message_size;
	readMessage(sockfd, &read_message_size, 1);
	readMessage(sockfd, buffer, read_message_size);
	if (buffer[0] == 0) {
		/* sunt jucatorul alb */
		player = White;
		printf("Sunt alb!\n");
	} else if (buffer[0] == 1) {
		/* sunt jucatorul negru */
		player = Black;
		printf("Sunt negru!\n");
	}else{
		// mesaj invalid; eroare!
	}
	
	BGBoard board; 
	Moves next_move;
	int dice1, dice2, n;

	while (true) 
	{
		bzero(buffer,256);
		
		readMessage(sockfd, &read_message_size, 1); 
		readMessage(sockfd, buffer, read_message_size); 

		if (read_message_size == 1) {
     	 	// Jocul s-a terminat;
      		if (buffer[0] == 'W') {
        		printf("WE WON\n");
      		} else {
        		printf("WE LOST\n");
      		}
      		break;
    	}

    	/* se efectueaza mutarea adversarului */
    	for (int i = 0; i < read_message_size-2; i+=2) {
    		board.move(OPPONENT(player),buffer[i],buffer[i+1]);
		}
		
		/* se extrage zarul din mesajul primit */
		dice1 = buffer[read_message_size-2];
    	dice2 = buffer[read_message_size-1];

    	/* se calculeaza mutarea optima pentru mine */
    	next_move = think(board,player,dice1,dice2);

		n = (int)next_move.size();

		/* daca nu am gasit nicio mutare valida, trimit mesaj de '0' bytes */
    	if (!n) {
    		sendMessage(sockfd, (char*) &n, 1); 
    		continue;
    	}

    	/* altfel se efectueaza mutarea */
    	bzero(buffer,256);
    	for (int i = 0; i < n; i+=2) {	
    		buffer[i] = (char)next_move[i];
    		buffer[i+1] = (char)next_move[i+1];
    		board.move(player,next_move[i],next_move[i+1]);
    	}
    	
    	/* se trimite la server un mesaj cu mutarea pe care vreau sa o fac */
		sendMessage(sockfd, (char*) &n, 1); 
    	sendMessage(sockfd, buffer, n); 
    	printf("Message sent!\n");
    }
	
	close(sockfd);
	return 0;
}
