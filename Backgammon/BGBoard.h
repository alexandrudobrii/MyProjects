#ifndef BGBoard_
	#define BGBoard_
	
	#include <iostream>
	#include <vector>
	
	#define BAR 30
	#define INF 100
	#define SIZE 24
	#define PICKW 0
	#define PICKB 25
	#define OPPONENT(x) ((x) == Black ? White : Black)
	
	using namespace std;

	/* enumerare cu jucatorii posibili */
	enum Player {
		Black,
		White,
		None
	};

	typedef vector <pair<Player,int> > Location;

/* clasa reprezentand tabla de joc cu toate metodele auxiliare */
class BGBoard {
  
  private:
  	/* tabla de joc este un vector de 26 de pozitii, pe fiecare pozitie afa
  	 * aflandu-se jucatorul si numarul de piese */
	Location board;
  	
  	/* numarul de piese din casa, respectic de pe bara pentru ambii jucatori */
  	int insideB, insideW, barB, barW;
  	
  public:
  	
  	/* se initializeaza tabla de joc */
	BGBoard() : insideB(5), insideW(5), barB(0), barW(0) { 
		
		board.resize(SIZE+2);
		
		for (int i = 1; i <= SIZE; i++)
		{
			board[i].first = None;	
			board[i].second = 0;
		}
		
		board[PICKW] = pair<Player,int>(White,0);
		board[PICKB] = pair<Player,int>(Black,0);

		board[1] = pair<Player,int>(Black,2);
		board[12] = pair<Player,int>(Black,5);
		board[17] = pair<Player,int>(Black,3);
		board[19] = pair<Player,int>(Black,5);
		
		board[24] = pair<Player,int>(White,2);
		board[13] = pair<Player,int>(White,5);
		board[8] = pair<Player,int>(White,3);
		board[6] = pair<Player,int>(White,5);
	}
	
	/* metoda ce returneaza jucatorul de pe pozitia specificata */
	inline Player get_player( int position ) {
		return board[position].first;
	}
	
	/* metoda ce returneaza numarul de piese de pe pozitia specificata */
	inline int get_number( int position ) const {
		return board[position].second;
	}
	
	/* metoda ce retuneaza numarul de pise din casa jucatorului specificat */
	inline int get_inside( Player player ) {
		return (player==White) ? insideW : insideB;
	}
	
	/* metoda ce returneaza numarul de piese de pe 
	 * bara pentru oponentul jucatorului specificat */
	inline int get_bar( Player player ) {
		return (player==White) ? barW : barB;
	}

	/* metoda ce returneaza 'true' daca jucatorul 'player' 
	 * poate scoate piese, 'false' altfel*/
	inline bool can_pick( Player player ) {
		return get_inside(player) == 15;
	}

	/* metoda ce returneaza 'true' daca se pot efectua mutari din casa pentru 
	 * pisele aflate in stanga valorii zarului, iar daca nu se gasesc piese in
	 * stanga se cauta prima piesa din dreapta; daca se poate efectua mutarea
	 * se returneaza 'true', altfel se returneaza 'false' */
	inline bool test( Player player, int from, int dice ) {

		switch (player)
		{
			case White:
				if (from >= dice) {
					return true;
				} else {
					for (int i = from+1; i <= 6; i++)
					{
						if (board[i].first == White)
							return false;
					}
					return true;
				}
				break;

			case Black:
				dice = SIZE-dice+1;
				
				if (from <= dice) {
					return true;
				} else {
					for (int i = from-1; i >= 19; i--)
					{
						if (board[i].first == Black)
							return false;
					}
					return true;
				}
				break;

			case None:
				break;
		}
		return false;
	}

	/* metoda care returneaza 'true' daca exista piese pe bara penru un 
	 * anumit jucator; altfel se returneaza 'false' */
	inline bool test_bar( Player player ) {
		return (player == White ? (barW > 0) : (barB > 0));
	}
	
	/* metoda care returneaza 'true' daca pozitia specificata este valida pentru 
	 * jucatorul specificat; altfel se returneaza 'false' */
	inline bool is_valid( Player player, int location ) {
		if (player == White)
			location = SIZE - location + 1;
	
		return ((board[location].first == player) || 
			((board[location].first != player) && (board[location].second < 2)))
			&& (location >= 1 && location <= 24);
	}

	/* metoda care returneaza 'true' daca se pot efectua mutari de la o anumita 
	 * pozitie un numar de 'how_many' casute; altfel se returneaza 'false' */
	inline bool is_valid( Player player, int from, int how_many ) {
		int location;
		
		if (player == White)
			location = from-how_many;
		else
			location = from+how_many;

		if (can_pick(player) && (location < 1 || location > 24)) {
			return true;
		}

		return ((board[location].first == player) || 
			((board[location].first != player) && (board[location].second < 2)))
			&& ((location >= 1 && location <= 24));
	}
	
	/* metoda care muta o piesa de la o pozitie un numar de 'how_many' casute */
	inline void move( Player player, int from, int how_many ) {	
		
		/* daca se muta din pozitia 30 atunci 
		 * se insereaza de pe bara */
		if (from == 30) {
			insert_from_bar(player,how_many);
			return;
		}

		/* se elimina o piesa din pozitia din care se muta */
		board[from].second--;
		if (!board[from].second)
			board[from].first = None;
		
		int to;
		switch (player) 
		{
			/* daca se muta o piesa pentru jucatorul negru */
			case Black:
				to = from + how_many;
				
				/* daca se muta in afara tablei de joc, dar se pot scoate
				 * piese atunci piesa respectiva este scoasa */
				if (to > 24 && can_pick(Black)) {
					board[PICKB].second++;
					return;
				}	

				/* daca piesa a fost introdusa in casa atunci se incrementeaza 
				 * numarul de piese din casa pentru jucatorul alb */	
				if (to >= 19 && from < 19) {
					insideB++;
				}

				/* daca se insereaza pe o pozitie valida unde se afla adversarul
				 * atunci piesa adversatului este scoasa pe bara */
				if (board[to].first == White) {
					barB++;
					board[to].second = 0;
					if (to <= 6)
						insideW--;
				}
				
				/* se muta piesa la pozitia specificata */
				board[to].first = Black;
				board[to].second++;
					
				break;
		
			/* daca se muta o piesa pentru jucatorul negru */
			case White:
				to = from - how_many;
				
				/* daca se muta in afara tablei de joc, dar se pot scoate
				 * piese atunci piesa respectiva este scoasa */
				if (to < 0 && can_pick(White)) {
					board[PICKW].second++;
					return;
				}

				/* daca piesa a fost introdusa in casa atunci se incrementeaza 
				 * numarul de piese din casa pentru jucatorul alb */
				if (to <= 6 && from > 6) {
					insideW++;
				}
				
				/* daca se insereaza pe o pozitie valida unde se afla adversarul
				 * atunci piesa adversatului este scoasa pe bara */
				if (board[to].first == Black) {
					barW++;
					board[to].second = 0;
					if (to >= 19)
						insideB--;
				}
				
				/* se muta piesa la pozitia specificata */
				board[to].first = White;
				board[to].second++;
					
				break;
			
			case None:
				break;
		}		
	}
	
	/* functie care insereza o piesa de pe bara pentru un anumit jucator */
	inline void insert_from_bar( Player player, int position ) {
		
		switch (player) 
		{
			/* daca se insereaza penru jucatorul negru */
			case Black:
				barW--;

				/* daca se insereaza intr-o locatie valida unde se afla 
				 * adversarul se scoate acea piesa a adversarului */
				if (board[position].first == White)
				{
					barB++;
					board[position].second = 0;
					insideW--;
				}
				break;
		
			/* daca se insereaza penru jucatorul alb */
			case White:
				barB--;

				/* daca se insereaza intr-o locatie valida unde se afla 
				 * adversarul se scoate acea piesa a adversarului */
				position = SIZE - position + 1;
				if (board[position].first == Black)
				{
					barW++;
					board[position].second = 0;
					insideB--;
				}
				break;
				
			case None:
				break;
		}
		
		/* se insereaza piesa */
		board[position].first = player;
		board[position].second++;
	}
};
	
	/* vector cu mutarea pe care am efectuat-o */
	typedef vector<int> Moves;

	/* pereche cu mutarea, respectiv tabla rezultata in urma mutarii */
	typedef pair<Moves,BGBoard> Solution;

	/* vector de solutii */
	typedef vector<Solution> Tree;

	ostream& operator<< ( ostream&, const BGBoard& );

	void gen_partial_moves(Solution, Player, int *, Tree&);
	
	void gen_partial_moves_double(Solution, Player, int, Tree&);
	
	int calc_score( BGBoard, Player);
	
	Moves think( BGBoard, Player, int, int);

#endif










 
