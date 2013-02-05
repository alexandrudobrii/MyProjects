#include "tema4.h"

int **alloc (int numtasks) {
	int **graph;

	graph = new int*[numtasks];
	for (int i = 0; i <  numtasks; i++) {
		graph[i] = new int[numtasks];
	}

	return graph;
}

std::string int_to_string (int number) {
	std::stringstream ss;
	
	ss << number;
	
	return ss.str();
}

int string_to_int (string number) {
	return atoi(number.c_str());
}

void write (int rank, int detour[], int numtasks) {
	char file[10];
	sprintf(file, "route_%d", rank);
	ofstream out(file);

	for (int i = 0; i < numtasks; i++) {
		out << i << ": " << detour[i] << endl;
	}
	out << endl;

	out.close();
}

/* functie parsarea unui sir de caractere */
void split_buffer (string line, vector<int>& buffer) {
	std::string token;
	std::stringstream ss(line);
	int number;

	while (ss >> token) {
		std::stringstream sis(token);
		sis >> number;
		buffer.push_back(number);
	}
}

/* functie ce intoarce sursa unui mesaj */
int get_source (std::string message) {
	int source, found;

	found = message.find(' ');
	source = string_to_int(message.substr(0, found));

	return source;
}

/* functie ce intoarce destinatia unui mesaj */
int get_destination (std::string message) {
	int found1, found2;
	std::string s;

	found1 = message.find(' ');
	found2 = message.find(' ', found1+1);
	s = message.substr(found1+1, found2-found1-1);

	if (s == "B") {
		return BROADCAST;
	}

	return string_to_int(s);
}

/* functie ce intoarce informatia unui mesaj */
std::string get_message (std::string message) {
	std::string mess;
	int found;

	found = message.find(' ');
	found = message.find(' ', found+1);
	
	mess = message.substr(found+1, message.size()-found-1);

	return mess;
}

/* functie care citeste din fisier topologia pentru fiecare nod in parte */
std::vector<int> read_topology (int rank, int numtasks, char *filename) {
	std::string line;
	std::ifstream in(filename);
	std::vector<int> buffer;
	
	for (int i = 0; i <= rank; i++) {
		getline(in, line);
	}
	in.close();

	split_buffer(line, buffer);
	
	return buffer;
}

/* functie ce creeaza un mesaj de trimis ulterior */
std::string create_message (int source, int destination, std::string info) {
	std::string message;

	message = int_to_string(source);
	message += " " + int_to_string(destination);
	message += " " + info;

	return message;
}

/* functie care realizeaza contorizarea voturilor */
std::pair<int, int> get_count (std::vector<int> leader, std::vector<int> adjunct) {
	int first = -1;
	int second = -1;
	int max;

	max = -1;
	for (unsigned int i = 0; i < leader.size(); i++) {
		if (leader[i] > max) {
			first = i;
			max = leader[i];
		}
	}

	max = -1;
	for (unsigned int i = 0; i < adjunct.size(); i++) {
		if (adjunct[i] > max) {
			second = i;
			max = adjunct[i];
		}
	}

	return make_pair(first, second);
}

/* se citesc din fisier mesajele de trimis */
std::vector<int> read_messages (int rank, char *filename, Messages& messages) {
	std::string from, to, line, to_send;
	std::ifstream in(filename);
	std::vector<int> buffer;
	int i, numtasks, from_int;

	in >> numtasks;
	for (i = 0; i < numtasks; i++) {
		in >> from >> to;
		getline(in, line);

		from_int = atoi(from.c_str());
		
		if (from_int == rank) {
			
			to_send = "";
			to_send += from;
			to_send += " " + to;
			to_send += line;
			messages.push_back(to_send);

			if (to == "B") {
				buffer.push_back(BROADCAST);	
			} else {
				buffer.push_back(atoi(to.c_str()));
			}
		}
	}

	in.close();

	return buffer;
}

/* se genereaza tabela de routare, utilizand algoritul Dijkstra */
void gen_routing_table (int **graph, int source, int detour[], int numtasks) {
	bool selected[numtasks];
	int parent[numtasks];
	int distance[numtasks];
	int u = -1, min;
	
	/* se initializeaza vectorii de parinte, respectiv de distante */
	for (int i = 0; i < numtasks; i++) {
		if (i == source) {
			distance[i] = 0;
			parent[i] = -1;
		} else if (graph[source][i] != NONE) {
			parent[i] = source;
			distance[i] = 1;
		} else {
			parent[i] = NONE;
			distance[i] = INF;
		}

		selected[i] = false;
	}

	selected[source] = true;
	
	/* de determina drumurile de la sursa la celelalte noduri */
	for (int i = 0; i < numtasks; i++) {
		min = INF;
		for (int j = 0; j < numtasks; j++) {
			if (distance[j] < min && !selected[j]) {
				min = distance[j];
				u = j;
			}
		}

		selected[u] = true;
		for (int nod = 0; nod < numtasks; nod++) {
			if (graph[u][nod] != NONE && !selected[nod]) {
				if (distance[nod] > distance[u] + 1) {
					distance[nod] = distance[u] + 1;
					parent[nod] = u;
				}
			}
		}
	}

	/* se determina tabela de rutare */
	detour[source] = source;
	for (int destination = 0; destination < numtasks; destination++) {

		if (destination == source) {
			continue;
		}

		for (int node = destination; true; node = parent[node]) {
			if (parent[node] == source) {
				detour[destination] = node;
				break;
			}
		}
	}
}

/* functie de care face broadcast in graph */
void MPI_MyBcast (string message, int detour[], int numtasks) {
	std::string new_mess;
	int source;

	source = get_source(message);

	for (int rank = 0; rank < numtasks; rank++) {

		if (source == rank) {
			continue;
		}

		new_mess.clear();
		new_mess = int_to_string(source);
		new_mess += " " + int_to_string(rank);
		new_mess += " " + get_message(message);

		MPI_Send((char *)new_mess.c_str(), 1400, MPI_CHAR, 
																				detour[rank], 1, MPI_COMM_WORLD);
	}
}

