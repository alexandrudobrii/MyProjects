#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstring>

#include "segmentation.h"

int main() {
	FlowGraph graph;
	vector<unsigned int> source_set;
  	vector<unsigned int> sink_set;
  	unsigned int source, sink;

	read_from_files();

	create_graph(graph);
	
	/* Calculam si afisam fluxul maxim de date care poate fi suportat de retea 
  	 * care este echivalent cu valoarea minima a fucntiei de energie */
   	source = graph.n-2;
  	sink = graph.n-1;
	double flow = maximum_flow(graph, source, sink);
  	printf("The maximum flow in the graph is: %f\n", flow);

  	/* Calculam si afisam o taietura minimala a grafului pentru a decide care
  	 * sunt pixelii care fac parte din prim-plan respectiv fundal */
  	min_cut(graph, source_set, sink_set, source);
 
 	/* se creeaza "segment.pgm" */
 	segment_image(source_set, sink_set);

 	return 0;
}

/* functie ce parseaza doua string-uri separate prin spatiu */
void parse_two (string line, int *x, int *y) {
	size_t found;

	found = line.find(' ');
	*x = atoi((line.substr(0,found).c_str()));
	*y = atoi((line.substr(found+1).c_str()));
}

/* se efectueaza citirea din fisier si memorarea datelor */
void read_from_files() {
	ifstream in, in1, in2;
	string line;
	
	/* se retin lambda si treshold */
	in.open("teste/11/parametri.txt");
	getline(in, line);
	parse_two(line, &lambda, &treshold);
	in.close();

	/* se retin in vectori pixelii celor 3 imagini */
	in.open("teste/11/imagine.pgm");
	in1.open("teste/11/mask_bg.pgm");
	in2.open("teste/11/mask_fg.pgm");
	
	getline(in, line);
	getline(in, line);
	getline(in, line);
	
	getline(in1, line);
	getline(in1, line);
	getline(in1, line);

	getline(in2, line);
	getline(in2, line);
	parse_two(line, &m, &n);
	getline(in2, line);
	
	/* se retin pixelii pentru fiecare poza in parte */
	for (int i = 0; i < m*n; i++) {
		getline(in, line);
		image.push_back(atoi(line.c_str()));

		getline(in1, line);
		if (line[0] != '0') {
			mask_bg.push_back(i);
		}

		getline(in2, line);
		if (line[0] != '0') {
			mask_fg.push_back(i);
		}
	}

	/* vector cu directii pentru a determina vecinii unui nod */
	v[0] = 1;
	v[1] = -1;
	v[2] = m;
	v[3] = -m;

	in.close();
	in1.close();
	in2.close();
}

/* functii ajutatoare pentru calculul capacitaii pe fiecare muchie */
double miu_f() {
	double sum = 0.0;

	for (unsigned int i = 0; i < mask_fg.size(); i++) {
		sum += image[mask_fg[i]];
	}

	return sum/mask_fg.size();
}

double miu_b() {
	double sum = 0.0;

	for (unsigned int i = 0; i < mask_bg.size(); i++) {
		sum += image[mask_bg[i]];
	}

	return sum/mask_bg.size();
}

double sigma_f() {
	double sum = 0.0;

	for (unsigned int i = 0; i < mask_fg.size(); i++) {
		sum += pow( miu_f()-image[mask_fg[i]], 2);
	}

	return sqrt(sum/mask_fg.size());
}

double sigma_b() {
	double sum = 0.0;

	for (unsigned int i = 0; i < mask_bg.size(); i++) {
		sum += pow( miu_b()-image[mask_bg[i]], 2);
	}

	return sqrt(sum/mask_bg.size());
}

/* functie care determina capacitatile pe muchiile dintre 
 * nodurile de start si finish si toate celelalte noduri */
double f (int i, int x) {
	double expr_f = 0.0, expr_b = 0.0;

	expr_f = 0.5 * pow((image[i]-miu_f())/sigma_f(), 2) + 
			 log( sqrt(2*PI*pow(sigma_f(),2)) );
	expr_f *= x;

	expr_b = 0.5 * pow((image[i]-miu_b())/sigma_b(), 2) + 
			 log( sqrt(2*PI*pow(sigma_b(),2)) );
	expr_b *= (1-x);

	return min( expr_f + expr_b, 10.0);
}

/* functie care determina capacitatile pe ficare muchie 
 * din graf,fara nodurile de strat respectiv finish */
 double f (int i, int j, int x1, int x2) {
	if (x1 != x2 && abs(image[i]-image[j]) <= treshold) {
		return lambda;
	} else {
		return 0.0;
	}
}

/* functie care determina daca o nodul curent('from') il are ca vecin pe 'to' */
bool is_valid (int from, int to) {
	return (to < 0 || to >= n*m || 
				(!(to%m) && (from == to-1)) || (!(from%m) && (to == from-1)));
}

/* functie care creeaza o lista de adiacenta pornind de la 'imagine.pgm' */
void create_graph (FlowGraph& graph)
{
	int new_pos;

	/* se retine numarul de noduri (inclusiv nodurile start si finish) */
	graph.n = n*m+2;

	/* Alocam graful. */
  	graph.capacity = new double*[graph.n];
  	for (unsigned int i = 0; i < graph.n; ++i) {
    	graph.capacity[i] = new double[graph.n];
    	memset(graph.capacity[i], 0x00, (graph.n) * sizeof(double));
    }
    graph.edges.resize(graph.n);

  	/* pentru fiecare nod din graf se retin intr-o lista de adiacenta 
  	 * vecinii si capacitatile pentru fiecare muchie in parte */
  	 for (unsigned int i = 0; i < graph.n-2; i++) {
		/* se leaga nodurile start si finish la toate 
		 * celelalte noduri; graful este neorientat */
		graph.edges[i].push_back(graph.n-2);
		graph.edges[graph.n-2].push_back(i);
		graph[i][graph.n-2] = f(i,1);
  		graph[graph.n-2][i] = graph[i][graph.n-2];

  		graph.edges[i].push_back(graph.n-1);
		graph.edges[graph.n-1].push_back(i);
  		graph[i][graph.n-1] = f(i,0);
  		graph[graph.n-1][i] = graph[i][graph.n-1];

  		/* se leaga toate nodurile (diferite de start si finish) 
  		 * conform cu datele citite din 'imagine.pgm' */
		for (int j = 0; j < 3; j++) {
  			new_pos = i + v[j];
  			if (!is_valid(i, new_pos)) {
  				graph.edges[i].push_back(new_pos);
  				graph.edges[new_pos].push_back(i);
  				graph[i][new_pos] = f(i,new_pos,0,1);
  				graph[new_pos][i] = graph[i][new_pos];
  			}
		}
	}
}

/* functie care creeaza 'segment.pgm' */
void segment_image (vector<unsigned int> source_set, 
					vector<unsigned int> sink_set) {
	ofstream out;
	
	/* se afiseaza in fisier Magic Number, numarul 
	 *  de linii si coloane, respectiv MaxValue */
	out.open("segment.pgm");
	out << "P2" << "\r\n";
	out << m << " " << n << "\r\n";	
	out << 255 << "\r\n"; 

	/* nodurile care aparitin lui source set (in urma 
	 * taieturii minimale)  fac pare din fundal (= 0) */
	for (unsigned int i = 0; i < source_set.size(); i++) {
		image[source_set[i]] = 0;
	}

	/* nodurile care aparitin lui source set (in urma 
	 * taieturii minimale) sunt in prim-plan (= 255) */
	for (unsigned int i = 0; i < sink_set.size(); i++) {
		image[sink_set[i]] = 255;
	}

	/* se afiseaza in fisier noii pixeli */
	for (unsigned int i = 0; i < image.size(); i++) {
		out << image[i] << " \r\n";
	}

	out.close();
}

