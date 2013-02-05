#include <iostream>
#include "colonii.h"

void print(char *file) {
	FILE *f = fopen(file, "w");

	for (int i = 0; i < iterations; i++) {
		fprintf(f, "%d %d %d %d\n", colA[i], pretA[i], colB[i], pretB[i]);
	}

	for (int i = 0; i < n ; i++) {
		for (int j = 0; j < n; j++) {
			fprintf(f, "(%d,%d,%d) ", 
						resurse_urm[i][j], pret_urm[i][j], buget_urm[i][j]);
		}
		fprintf(f,"\n");
	}

	fclose(f);
}

/* functie care realizeaza citirea din fisier a matricilor */
void read_from_file (char *file) {
	ifstream in;
	
	in.open(file);
	in >> pmin;
	in >> pmax;
	in >> n;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			in >> resurse_urm[i][j]; 
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			in >> pret_urm[i][j];
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			in >> buget_urm[i][j];
		}
	}
}

/* functie care intorce distanta Manhattan dintre doua puncte */
int get_distance (int i, int j, int i1, int j1) {
	return abs(i-i1) + abs(j-j1);
}

/* functie care intoarce costul unei resurse */
int get_cost (int i, int j, int i1, int j1) {
	return pret[i1][j1] + get_distance(i,j,i1,j1);
} 

/* functie care determina costurile maxime pentru A si B */
void set_values (int an) {
	int pret_maxA = -1, pret_maxB = -1;
	int sum1 = 0, sum2 = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			
			/* se determina numarul de resurse de tipul A */
			if (resurse_urm[i][j] == 0) {
				sum1 += 1;
				if (pret_maxA < pret_urm[i][j]) {
					pret_maxA = pret_urm[i][j];
				}
			}

			/* se determina numarul de resurse de tipul A */
			if (resurse_urm[i][j] == 1) {
				sum2 += 1;
				if (pret_maxB < pret_urm[i][j]) {
					pret_maxB = pret_urm[i][j];
				}
			}
		}
	}

	colA[an] = sum1;
	colB[an] = sum2;
	pretA[an] = pret_maxA;
	pretB[an] = pret_maxB;
}

/* functie care modifica evolutia pietei pentru un numar de ani */
void calculate() {
	int pret_maxA, pret_maxB, cost, cost_res;

	for (int an = 0; an < iterations; an++) {
		
		/* se updateaza matricile cu valorile din noul an */
		for (int i1 = 0; i1 < n; i1++) {
			for (int j1 = 0; j1 < n; j1++) {
				resurse[i1][j1] = resurse_urm[i1][j1];
				pret[i1][j1] = pret_urm[i1][j1];
				buget[i1][j1] = buget_urm[i1][j1];
			}	
		}
		
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				
				/* se calculeaza costul minim pentru Col_ij */
				cost = MAXIM;
				cost_res = MAXIM;
				for (int k = 0; k < n; k++) {
					for (int l = 0; l < n; l++) {
					
						if (resurse[i][j] != resurse[k][l] 
										   && get_cost(i,j,k,l) < cost) {
							cost = get_cost(i,j,k,l);
						}
						
						if (get_cost(i,j,k,l) < cost_res
									 &&	resurse[i][j] == resurse[k][l]) {
							cost_res = get_cost(i,j,k,l);
						}
					}
				}

				/* se actualizeaza pretul si bugetul */
				if (cost > buget[i][j]) {
					pret_urm[i][j] = pret[i][j] + cost - buget[i][j];
					buget_urm[i][j] = cost;
				} else if (cost < buget[i][j]) {
					pret_urm[i][j] = pret[i][j] + floor((cost - buget[i][j])/2);
					buget_urm[i][j] = cost;
				} else {
					pret_urm[i][j] = cost_res + 1;
					buget_urm[i][j] = cost;
				}

				if (pret_urm[i][j] < pmin) {
					pret_urm[i][j] = max(pret_urm[i][j], pmin);
				}

				/* se actualizeaza pretul, bugetul si resursele */
				if (pret_urm[i][j] > pmax) {
					resurse_urm[i][j] = 1 - resurse[i][j];
					buget_urm[i][j] = pmax;
					pret_urm[i][j] = floor((pmin + pmax)/2);
				}
			}
		}

		/* se calculeaza valorile maxime pentru resursele A si B */
		set_values(an);
	}
}

int main (int argc, char *argv[]) {

	if (argc != 4) {
		cout << "Usage: ./exec T file_in file_out" << endl;
		return 0;
	}

	iterations = atoi(argv[1]);
	
	read_from_file(argv[2]);	

	calculate();

	print(argv[3]);

	return 0;
}