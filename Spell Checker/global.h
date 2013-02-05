
#ifndef GLOBAL_
	#define GLOBAL_
	
	#include <string>
	#include <vector>
	
	using namespace std;
	
	/* structura pentru memorarea dictionarului */
	class Dictionary {
	  public:
		string cuvant;
		int frecventa;
	
		Dictionary( string cuvant, int frecventa ) : cuvant(cuvant),
					frecventa(frecventa) {}
	};
  
	/* structura pentru memorarea solutiilor generate */
	class Optim {
	  public:
		string cuvant;
		int frecventa;
		int abatere;
		int nrCuvinte;
		
		Optim () {
			nrCuvinte = 1;
			frecventa = 0;
			abatere = 65;
		}
		
		Optim( string cuvant, int frecventa, int abatere ) :
				cuvant(cuvant), frecventa(frecventa), abatere(abatere) 
		{
			nrCuvinte = 1;
		}
				
		Optim( string cuvant, int frecventa, int abatere, int nrCuvinte ) :
				cuvant(cuvant), frecventa(frecventa), abatere(abatere),
				nrCuvinte(nrCuvinte) {}
		
		int addAbat( Optim *sol ) {
			return this->abatere+sol->abatere;
		}
		
		int addFrecv( Optim *sol ) {
			return this->frecventa+sol->frecventa;
		}
		
		inline bool operator< ( const Optim& sol ) const {
			
				 /* conditia 1 */
			if ((this->abatere < sol.abatere) || 
				
				 /* conditia 2 */
			   (this->abatere == sol.abatere && this->nrCuvinte < sol.nrCuvinte) || 
			
				 /* conditia 3 */
				(this->abatere == sol.abatere && this->nrCuvinte == sol.nrCuvinte &&
				 this->frecventa > sol.frecventa) ||
				
				 /* conditia 4 */
				(this->abatere == sol.abatere && this->nrCuvinte == sol.nrCuvinte && 
				this->frecventa == sol.frecventa && (sol.cuvant.compare(this->cuvant) > 0)))
				
					return true;
			
			return false;
		}
	};
	
	/* matrice pentru memorarea distantelor dinte 
	 * cuvinte, utilizand algoritmul Levenstein	*/
	int dist[65][65];
	
	/* vector pentru memorarea cuvintelor din dictionar */
	vector<Dictionary *> words;
	
	/* matrice pentru memorarea solutiilor generate */
	vector< vector<Optim *> > solution(65, vector<Optim *>(65));
	
	/* vector pentru memorarea solutiilor optime la fiecrea pas */
	vector<Optim *> optim(65);
	
#endif