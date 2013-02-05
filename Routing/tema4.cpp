#include "tema4.h"

/* ========================================================================== */

int main( int argc, char *argv[])  {
	int numtasks, parinte = NONE, rank, destination, tag = 1;
	char inmsg, outmsg = 'x', buf[1401];
	MPI_Status Stat;
	std::vector<int> tokens;
	Messages message;
	std::string recv;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int graph[numtasks][numtasks];
	int buffer[numtasks][numtasks];
	int detour[numtasks];
	int **graph_aux = alloc(numtasks); 

	/* citire din fisier */
	for (int i = 0; i < numtasks; i++) {
		for (int j = 0; j < numtasks; j++) {
			graph[i][j] = 0;
		}
	}

	/* fiecare nod isi citeste propria linie din fisier */
	tokens = read_topology(rank, numtasks, argv[1]);
	for (unsigned int i = 1; i < tokens.size(); i++) {
			graph[rank][tokens[i]] = 1;
	}


/* ========================================================================== */

	if (rank == MASTER) {
		int vecini = 0;

		/* se trimit sondaje vecinilor */
		for (int i = 0; i < numtasks; i++) {
			if (graph[rank][i] == 1) {
				MPI_Send(&outmsg, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
				vecini++;
			}
		}

		/* se primeste matricea de adiacenta de la vecini */
		for (int k = 0; k < vecini; k++) {
			MPI_Recv(buffer, numtasks*numtasks, MPI_INT, 
																		MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);
			
			for (int i = 0; i < numtasks; i++) {
				for (int j = 0; j < numtasks; j++) {
					graph[i][j] = graph[i][j] || buffer[i][j];
				}
			}
		}

		/* se genereaza tabela de routare si se trimite celolalte noduri din graf */
		for (int i = 0; i < numtasks; i++) {
			for (int j = 0; j < numtasks; j++) {
				graph_aux[i][j] = graph[i][j];
			}
		}

		gen_routing_table(graph_aux, rank, detour, numtasks);
		write(rank, detour, numtasks);

		/* se face broadcast */
		for (int i = 1; i < numtasks; i++) {
			MPI_Send(graph, numtasks*numtasks, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
	} 

	if (rank != MASTER) {
		int vecini = 0;
	
		/* se primesc sondaje */	
		MPI_Recv(&inmsg, 1, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);
		parinte = Stat.MPI_SOURCE;
		
		/* se trimit mai departe sondaje vecinilor */
		for (int i = 0; i < numtasks; i++) {
			if (graph[rank][i] == 1 && i != parinte) {
				MPI_Send(&outmsg, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
				vecini++;
			}
		}

		/* se primesc ecouri de la vecini */
		for (int k = 0; k < vecini; k++) {
			MPI_Recv(buffer, numtasks*numtasks, MPI_INT, 
																		MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);
			
			for (int i = 0; i < numtasks; i++) {
				for (int j = 0; j < numtasks; j++) {
					graph[i][j] = graph[i][j] || buffer[i][j];
				}
			}
		}
		
		/* se trimit ecouri vecinilor */
		MPI_Send(graph, numtasks*numtasks, MPI_INT, parinte, tag, MPI_COMM_WORLD);
		
		/* se primeste matrcea de adiacenta finala */
		MPI_Recv(graph, numtasks*numtasks, MPI_INT, 
																		MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);

		/* se genereaza tabela de routare */
		for (int i = 0; i < numtasks; i++) {
			for (int j = 0; j < numtasks; j++) {
				graph_aux[i][j] = graph[i][j];
			}
		}

		gen_routing_table(graph_aux, rank, detour, numtasks);
		write(rank, detour, numtasks);
	}

	MPI_Barrier(MPI_COMM_WORLD);


/* ========================================================================== */

	bool once = true;
	int count = 0;

	/* se citesc din fisier mesajele de transmis */
	tokens = read_messages(rank, argv[2], message);
	
	while (1) {

		/* se trimit toate mesajele la destinatiile corespunzatoare */
		for (unsigned int i = 0; i < tokens.size(); i++) {
			
			if (tokens[i] != BROADCAST) {
				MPI_Send((char *)message[i].c_str(), 1400, MPI_CHAR, 
																				detour[tokens[i]], tag, MPI_COMM_WORLD);
				
				printf("rank = %d, destination = %d, next_hop = %d, message = %s\n", 
					 rank, tokens[i], detour[tokens[i]], get_message(message[i]).c_str());

			} else {
				MPI_MyBcast(message[i], detour, numtasks);
			}

			if (i == tokens.size()-1) {
				tokens.clear();
			}
		}

		/* daca nu mai exista niciun mesaj de trimis, 
			se trimit mesaje de terminare la nodul MAASTER */
		if (tokens.size() == 0 && once) {
			std::string finish;
			
			finish = create_message(rank, MASTER, "AM TERMINAT!"); 

			MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																				detour[MASTER], tag, MPI_COMM_WORLD);

			once = false;
		}

		/* se primesc toate mesajele */
		MPI_Recv(&buf, 1400, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);
		
		recv = buf;
		destination = get_destination(recv);
		
		printf("rank = %d, destination = %d, next_hop = %d, message = %s\n", 
						rank, destination, detour[destination], get_message(recv).c_str());
	
		/* daca mesajul nu a ajuns la destiatie se routeaza mai departe */
		if (destination != rank) {
			
			MPI_Send(buf, 1400, MPI_CHAR, detour[destination], tag, MPI_COMM_WORLD);
		
		/* daca mesajul a ajuns la destinatie */
		} else {

			printf("rank = %d, message = %s, Am ajuns la destinatie!!!\n", 
																							rank, get_message(recv).c_str());
			
			/* daca mesajul este de tipul 'EXIT' nodul care l-a primit trimte
			   vecinilor mesaje cu acelasi mesaj si isi incheie activitatea */
			if (get_message(recv) == "EXIT!") {
				std::string finish;

				for (int i = 0; i < numtasks; i++) {
					if (graph[rank][i] == 1 && i != Stat.MPI_SOURCE) {
							finish = create_message(rank, i, "EXIT!");

							MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																										i, tag, MPI_COMM_WORLD);
					}
				}
				break;
			}

			/* daca nodul MASTER primeste mesaje 'AM TERMINAT', acestea se 
			   contorizeaza pana cand stie ca a primit astfel de mesaje de la 
			   toate celelalte noduri din graf si trimite mesaje cu tipul EXIT */
			if (rank == MASTER && get_message(recv) == "AM TERMINAT!") {
				count++;

				if (count == numtasks) {
					std::string finish;
			
					for (int i = 0; i < numtasks; i++) {
						if (graph[rank][i] == 1) {
							finish = create_message(rank, i, "EXIT!");

							MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																											i, tag, MPI_COMM_WORLD);
						}
					}
					break;
				}
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

/* ========================================================================== */

	int first, second;
	std::vector<int> leader(numtasks), adjunct(numtasks);
	once = true;
	count = 0;
	srand(time(NULL));

	while (1) {
		
		/* se trimite la nodul MASTER decizia fiecarui nod */
		if (once) {
			std::string finish;
			std::string message;
			
			first = rand()%numtasks;
			while (first == rank) {
				 first = rand()%numtasks;
			}
			
			second = rand()%numtasks;
			while (second == first || second == rank) {
				second = rand()%numtasks;
			}

			printf("Rank %d voteaza pentru: leader - %d, adjunct - %d\n", 
																													rank, first, second);
			
			message = int_to_string(first);
			message += " " + int_to_string(second);

			finish = create_message(rank, MASTER, message); 

			MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																					detour[MASTER], tag, MPI_COMM_WORLD);

			once = false;
		}

		/* se realizeaza primirea de mesaje */
		MPI_Recv(&buf, 1400, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &Stat);
		
		recv = buf;
		destination = get_destination(recv);
		
		/* daca mesajul nu a ajuns la destinatie, acesta se routeaza mai departe */
		if (destination != rank) {
			
			MPI_Send(buf, 1400, MPI_CHAR, detour[destination], tag, MPI_COMM_WORLD);
		
		/* daca mesajul a ajuns la destinatie... */
		} else {
			
		/* daca mesajul este de tipul 'EXIT' nodul care l-a primit trimte
			 vecinilor mesaje cu acelasi mesaj si isi incheie activitatea */	
			if (get_message(recv) == "EXIT!") {
				std::string finish;

				for (int i = 0; i < numtasks; i++) {
					if (graph[rank][i] == 1 && i != Stat.MPI_SOURCE) {
							finish = create_message(rank, i, "EXIT!");

							MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																										i, tag, MPI_COMM_WORLD);
					}
				}
				break;
			}

			/* se centralizeaza datele cu votarile fiecarui nod */
			if (rank == MASTER) {
				std::string finish;
				std::string message;
				std::pair<int, int> decision;
 				
 				count++;

 				if (count != numtasks) {
 				
					leader[get_source(get_message(recv))]++;
 					adjunct[get_destination(get_message(recv))]++; 
 				
 				} else {
 				
					leader[get_source(get_message(recv))]++;
 					adjunct[get_destination(get_message(recv))]++; 					
 					decision = get_count(leader, adjunct);

					message = int_to_string(decision.first);
					message += " " + int_to_string(decision.second); 					
 					finish = create_message(rank, MASTER, message);

 					/* se trimite celorlalte noduri un mesaj cu decizia finala */
 					MPI_MyBcast(finish, detour, numtasks);

 					printf("Exit polll!!! Rank %d stie ca: leader - %d, adjunct - %d\n", 
 																													rank, first, second);
 					/* fiecare nod isi incheie activitatea */
	 				for (int i = 0; i < numtasks; i++) {
						if (graph[rank][i] == 1) {
							finish = create_message(rank, i, "EXIT!");

							MPI_Send((char *)finish.c_str(), 1400, MPI_CHAR, 
																											i, tag, MPI_COMM_WORLD);
						}
					}
					break;
				}
			}

			/* se primeste rezultatul final si se afiseaza */
			if (rank != MASTER) {
					first = get_source(get_message(recv));
					second = get_destination(get_message(recv));

				printf("Exit polll!!! Rank %d stie ca: leader - %d, adjunct - %d\n", 	
																													rank, first, second);
			}
		}
	}

	MPI_Finalize();
	
}

/* ========================================================================== */
