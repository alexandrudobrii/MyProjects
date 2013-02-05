	/* Nume: Dobrii Ionut-Alexandru 
	 * Grupa si seria: 324CA */

#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>

#include "global.h"
 
/* functie pentru initializare a variabilelor globale */
void init_global() {
	int i, j;
	
	for (i = 0; i < 18; i++)
		dist[i][0] = i;
	for (i = 0; i < 18; i++)
		dist[0][i] = i;
	
	for (i = 0; i < 65; i++)
		for (j = 0; j < 65; j++)
			solution[i][j] = new Optim();
}

/* functie care citeste si memoreaza datele din fisier */
void read_from_file() {
	string text, str;
	size_t found;
	ifstream infile;
	int frecv;
	
	infile.open("dict.txt");
	
	while (infile.good())
	{
		getline(infile,text);
        found = text.find(" ");
		str = text.substr(0,found);
		frecv = atoi(text.substr(found+1).c_str());
		
		words.push_back(new Dictionary(str,frecv));
	}
	
	infile.close();
}

/* functie care calculeaza abaterea minima dintre 2 cuvinte.
 * In acelasi timp se caluleaza distanta minima si pentru
 *	toate sufixele unui cuvant */
void get_distance( string s, string t, int frecventa, int col ) {
	int i, j, n, m;
	
	m = s.length();
	n = t.length();

	/* se retin in matricea 'dist' toate distantele dintre toate
	 * prefixele primului string si toate prefixele celui de-al doilea */
	for (i = 1; i < m+1; i++ )
    {
		if (i > 13) break;
		 
		for (j = 1; j < n+1; j++)
			if (s[i-1] == t[j-1])
				dist[i][j] = dist[i-1][j-1];
			else
				dist[i][j] = min( min( dist[i-1][j]+1,dist[i][j-1]+1 ),dist[i-1][j-1]+1 );
		
		/* se retin in matricea 'solution' toate sufixele optime primului 
		 * cuvant in cazul in care acestea au abaterile mai mici decat cele
		 * calculate in pasii anteriori; in caz de abateri egale se compara 
		 * in functie de frecvente */	
		if (dist[i][n] < solution[i-1+col][col]->abatere || 
			((dist[i][n]) == solution[i-1+col][col]->abatere 
			&& solution[i-1+col][col]->frecventa < frecventa)) 
		{	
				solution[i-1+col][col] = new Optim(t,frecventa,dist[i][n]);
		}
	}
}
 
/* functie care genereaza cu ajutorul algoritmului lui Levenstein 
 * toate string-urile corecte porinind de la substring-urile sirului 
 * initial. Toate aceste solutii se memoreaza in matricea 'solution' */
void gen_all_solutions( string sir, int lungimeSir ) {
	vector<Dictionary *>::iterator it;
	string s;
	int i;
	
	/* se parccurge sirul citit */
	i = 0;
	while (i < lungimeSir)
	{
		/* se parcurge dictioanrul de cuvinte */
		s = sir.substr(i,lungimeSir);
		it = words.begin();
		while (it != words.end())
		{
			/* se genereaza solutii valide pornind de	
			 * la toate subisiruruile sirului initial */
			get_distance(s, (*it)->cuvant, (*it)->frecventa, i );
			it++;
		}
		i++;
	}
}

/* functie de combinare a tuturor solutiilor si determinare a
 * a sirului optim pentru sirul citit de la tastatura */
string test_all_solutions( int lungimeSir ) {
	string temporaryString;
	int i, j;
	Optim *temp;

	/* se parcurge matricea solutiilor */
	i = 0;
	while (i < lungimeSir)
	{
		/* se combina toate solutiile de pe fiecare linie. 
		 * re retine primul cuvant de pe fiecare linie */
		optim[i] = solution[i][0];
		
		j = 0;
		while (j < i) 
		{
			/* se combina toate solutiile de pe fiecare linie 
			 * inserandu-se spatii intre acestea */
			temporaryString = optim[j]->cuvant + " " + solution[i][j+1]->cuvant;
			
			/* se creeaza o noua solutie care contine sirul creat anterior,
			 * cu suma dintre frecventele  abaterilor optime pana la pasul 'j'
			 * cu  */
			temp = new Optim(temporaryString, optim[j]->addFrecv(solution[i][j+1]),
							 optim[j]->addAbat(solution[i][j+1]), optim[j]->nrCuvinte+1);
			
			/* in 'global.h' am suprascris operatorul '<' astfel incat sa compare
			 * doua instante ale clasei 'Optim' dupa abatere. In caz de abatere
			 * egala se compara dupa numarul de cuvinte. Daca si in acest caz exista
			 * mai multe posibilitati se compara dupa frecventa. In ultiul rand	
			 * se compara astfel incat sirul obtinut sa fie minim lexicografic.
			 * Daca toate una din conditiile de mai sus este indeplinita se 
			 * actualizeaza solitia optima */
			if (*temp < *optim[i])
				optim[i] = temp;
			j++;
		}
		i++;
	}
	
	/* se returneaza ultima soltuie gasita, 
	 * intrucat aceasta este cea optima */
	return optim[lungimeSir-1]->cuvant;
}
			
int main()
{
	int lungimeSir;
	
	/* se citeste sirul de corecatat de la tastatura */
	string decorectat, correct;;
	getline(cin, decorectat);
	
	/* se elimina spatiile albe din string-ul citit */
	decorectat.erase(remove(decorectat.begin(), decorectat.end(), ' '), decorectat.end());	
	lungimeSir = decorectat.length();
	
	/* se initializeaza variabilele globale */
	init_global();
	
	/*se memoreaza intr-un vector toate cuvintele 
	 * si frecventele citite din fisier */
	read_from_file();
	
	/* se genereaza toate combinatiile de solutii optime pornind
	 * de la sirul citit de la tastaura dupa eliminarea spatiilor albe */
    gen_all_solutions(decorectat,lungimeSir);
	
	/* se testeaza solutiile generate, la final ramanand solutia optima */
	correct = test_all_solutions(lungimeSir);
	
	cout << correct << endl;
	
	return 0;
}

