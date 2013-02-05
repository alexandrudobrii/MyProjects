#include "BGBoard.h"
#include <algorithm>

/* functie care genereaza mutari cu toate combinatiile posibile de a scoate 
 * piese pentru jucatorul alb */
void pick_checker_White(Solution s, Player player, int dice[], Tree& tree) {
	BGBoard board, final_board;
	Solution sol, final_sol;
	bool moved;
	
	/* se parcurge casa jucatorului alb */
	for (int i = 6; i >= 1; i--)
	{
		sol = s;
		board = sol.second;

		/* daca exista vreo mutare valida unui zar */
		if (board.get_player(i) == White && board.test(player,i,dice[0]) &&
					board.is_valid(player,i,dice[0]))
		{
			/* se adauga la multimea de solutii mutarea facuta */
			board.move(player,i,dice[0]);
			sol.first.push_back(i);
			sol.first.push_back(dice[0]);
			sol.second = board;
		
			/* se parcurge din nou casa jucatorului alb */
			moved = false;
			for (int j = 6; j >= 1; j--)
			{
				/* daca exista mutari valide si pentru zarul ramas */	
				if (board.get_player(j) == White && board.test(player,j,dice[1]) 
							&& board.is_valid(player,j,dice[1]))
				{
					/* se actualizeaza tabla de joc si se adauga la multimea
					 * de solutii mutarea facuta  */
					moved = true;
					final_sol = sol;
					final_sol.first.push_back(j);
					final_sol.first.push_back(dice[1]);

					final_board = board;
					final_board.move(player,j,dice[1]);
					final_sol.second = final_board; 

					tree.push_back(final_sol);
				}
			}
			/* daca s-au gasit mutari valide pentru primul zar, dar nu s-au 
			 * gasit mutari valida pentru cel de-al doilea zar se adauga
			 * la multimea de solutii, mutarea intermediara */
			if (!moved)
				tree.push_back(sol);
		}
	}
}

/* functie care genereaza mutari cu toate combinatiile posibile de a scoate 
 * piese pentru jucatorul negru */
void pick_checker_Black(Solution s, Player player, int dice[], Tree& tree) {
	BGBoard board, final_board;
	Solution sol, final_sol;
	bool moved;

	/* se parcurge casa jucatorului negru */
	for (int i = 19; i <= 24; i++)
	{
		sol = s;
		board = sol.second;

		/* daca exista vreo mutare valida unui zar */
		if (board.get_player(i) == Black && board.test(Black,i,dice[0]) &&
					board.is_valid(Black,i,dice[0]))
		{
			/* se adauga la multimea de solutii mutarea facuta */
			board.move(player,i,dice[0]);
			sol.first.push_back(i);
			sol.first.push_back(dice[0]);
			sol.second = board;
		
			/* se parcurge din nou casa jucatorului negru */
			moved = false;
			for (int j = 19; j <= 24; j++)
			{
				/* daca exista mutari valide si pentru zarul ramas */	
				if (board.get_player(j) == Black && board.test(Black,j,dice[1]) 
							&& board.is_valid(Black,j,dice[1]))
				{
					/* se actualizeaza tabla de joc si se adauga la multimea
					 * de solutii mutarea facuta  */
					moved = true;
					final_sol = sol;
					final_sol.first.push_back(j);
					final_sol.first.push_back(dice[1]);

					final_board = board;
					final_board.move(player,j,dice[1]);
					final_sol.second = final_board; 

					tree.push_back(final_sol);
				}
			}
			/* daca s-au gasit mutari valide pentru primul zar, dar nu s-au 
			 * gasit mutari valida pentru cel de-al doilea zar se adauga
			 * la multimea de solutii, mutarea intermediara */
			if (!moved)
				tree.push_back(sol);
		}
	}
}

/* functie care insereaza in casa adversarului doua piese de pe bara */
void insert_two(Solution s, Player player, int dice[], Tree& tree) {
	BGBoard board = s.second;
	
	/* daca exista mutare valida de inserare
	 * pentru primul zar */
	if (board.is_valid(player,dice[0]))
	{
		/* se efectueaza mutarea si se adauga 
		 * la multimea de solutii */
		board.insert_from_bar(player,dice[0]);
		s.first.push_back(BAR);
		s.first.push_back(dice[0]);
		s.second = board;
		tree.push_back(s);
	}

	/* daca exista mutare valida de inserare
	 * si pentru cel de-al doilea zar */
	if (board.is_valid(player,dice[1]))
	{
		/* se efectueaza mutarea si se adauga 
		 * la multimea de solutii */
		board.insert_from_bar(player,dice[1]);
		s.first.push_back(BAR);
		s.first.push_back(dice[1]);
		s.second = board;
		tree.push_back(s);
	}
}

/* functie care muta o piesa din afara casei in casa conform primului zar si
 * scoate o piesa din casa unui anumit jucator, conform celui de-al doilea zar*/
void move_and_pick(BGBoard board, Solution sol, 
									Player player, int dice[], Tree& tree) {
	Solution final_sol;
	BGBoard final_board;

	/* daca se efectueaza mutarea pentru jucatorul alb */
	if (player == White)
	{
		/* se parcurge casa jucatorului alb */
		for (int i = 6; i >= 1; i--)
		{
			/* daca exista posibilitatea de a muta piese in casa  */
			if (board.get_player(i) == White && 
		   	   board.test(player,i,dice[0]) && board.is_valid(player,i,dice[0]))
			{
				/* se efectueaza mutarea si se adauga la multimea de solutii */
				final_sol = sol;
				final_sol.first.push_back(i);
				final_sol.first.push_back(dice[0]);
					
				final_board = board;
				final_board.move(player,i,dice[0]);
				final_sol.second = final_board;
					
				tree.push_back(final_sol);
			}
		}
	}

	/* daca se efectueaza mutarea pentru jucatorul negru */
	if (player == Black)
	{
		/* se parcurge casa jucatorului negru */
		for (int i = 19; i <= 24; i++)
		{
			/* daca exista posibilitatea de a muta piese in casa  */
			if (board.get_player(i) == Black && board.test(player,i,dice[0]) &&
										board.is_valid(player,i,dice[0]))
			{
				/* se efectueaza mutarea si se adauga la multimea de solutii */
				final_sol = sol;
				final_sol.first.push_back(i);
				final_sol.first.push_back(dice[0]);
					
				final_board = board;
				final_board.move(player,i,dice[0]);
				final_sol.second = final_board;
					
				tree.push_back(final_sol);
			}
		}
	}
}

/* functie de generare de mutari valida pentru un zar dat 
 * input: => tabla de joc ('Solution')
 *        => jucatorul care muta ('player') 
 *        => zarul ('dice[]')
 * output: <= multimea cu mutari valida ('tree') */
void gen_partial_moves(Solution s, Player player, int dice[], Tree& tree) {
	BGBoard board, final_board;
	Solution sol, final_sol;
	bool inserted = false;
	int nr = 0, nr2 = 0;

	board = s.second;
	
	/* daca se pot scoate piese se efectueaza mutarea 
	 * respectiva pentru jucatorul care muta */
	if (board.can_pick(player))
	{		
		if (player == White) 
			pick_checker_White(s,White,dice,tree); 

 		if (player == Black)  
			pick_checker_Black(s,Black,dice,tree);

		return;
	}

	/* daca se doua piese sunt scoase si trebie
	 * se fie introduse in casa, se incearca 
	 * repunerea acestora in joc */
	if (board.get_bar(OPPONENT(player)) >= 2)
	{ 
		insert_two(s,player,dice,tree);
		return;
	}

	/* daca se afla o piesa pe bara se inceraca 
	 * repunerea in joc; in cazul afirmariv se
	 * efectueaza mutarea conform unui zar si se
	 * se cauta mutari valide pentru celalalt zar */
	sol = s;
	if (board.get_bar(OPPONENT(player)) == 1)
	{
		if (!board.is_valid(player,dice[0]))
			return;
		
		board.insert_from_bar(player,dice[0]);
		
		sol.first.push_back(BAR);
		sol.first.push_back(dice[0]);
		sol.second = board;
		inserted = true;
	}
	
	/* se parcurge tabla de joc */
	nr = 0;
	for (int i = 1; i <= SIZE; i++)
	{	
		/* daca nu s-a inserat nicio piese
		 * se reactalizeaza solutia curenta */
		if (!inserted)
		{
			sol = s;
			board = s.second;
		}
		
		/* daca exista mutari valide conform unui zar */
		if ((board.get_player(i) == player) && board.is_valid(player,i,dice[1]))			
		{
			/* daca anterior nu s-a inserat nicio piesa
			 * se efectueaza mutari intermediare penru un zar */
			if (!inserted)
			{
				sol.first.push_back(i);
				sol.first.push_back(dice[1]);
				board.move(player,i,dice[1]);
				sol.second = board;
			}
			/* alffel, se adauga la multimea de solutii mutarile finale */
			else
			{
				final_sol = sol;
				final_sol.first.push_back(i);
				final_sol.first.push_back(dice[1]);
				
				final_board = board;
				final_board.move(player,i,dice[1]);
				final_sol.second = final_board;
				
				tree.push_back(final_sol);
			}
			
			/* se parcurge din nou tabla de joc */
			nr2 = 0;
			for (int j = 1; j <= SIZE && !inserted; j++)	
			{
				/* daca jucatorul a mutat si acum are de scos,se 
				 * efectueaza mutarea si aduaga la multimea de solutii */
				if (player == White && board.can_pick(White))  
				{
					move_and_pick(board,sol,White,dice,tree);
					break;
				}

				if (player == Black && board.can_pick(Black))  
				{
					move_and_pick(board,sol,Black,dice,tree);
					break;
				}
				 
				/* daca se mai pot efectua mutari valide pentru a doilea zar */
				if ((board.get_player(j) == player) && 
											board.is_valid(player,j,dice[0]))
				{
					/* se adauga mutarea respectiva la multimea de solutii */
					final_sol = sol;
					final_sol.first.push_back(j);
					final_sol.first.push_back(dice[0]);
					
					final_board = board;
					final_board.move(player,j,dice[0]);
					final_sol.second = final_board;
					
					tree.push_back(final_sol);
				}
				else
				{
					nr2++;
				}
			}
			/* daca s-a efectuat o mutare pentru primul zar,
			 * dar nu s-a efectuat nicio mutare pentru zarul 
			 * al doilea, se adauga la multimea de solutii
			 * mutarea intermediara */
			if (nr2 == SIZE)
				tree.push_back(sol);
		}
		else
		{
			nr++;
		}
	}
	/* daca s-a inserat o piesa de pe bara si nu s-au mai efectuat 
	 * alte mutari, se actualizeaza multimea solutiilor */
	if (nr == SIZE && inserted)
		tree.push_back(sol);
}

/* functie care genereaza toate posibilitatile de a muta pentru o tabla si
 * si o pereche de zaruri date; 
 * input: => tabla de joc
 * 		  => jucatorul ( White/Black )
 		  => perechea de zaruri primita de la server	
 * output: <= vector cu toate posibilitatile de mutare */
Tree gen_all_moves(BGBoard board, Player player, int dice1, int dice2) {
	Tree tree, final_tree, tree2;
	Moves move;
	Solution sol(move,board);
	int dice[2];
	
	dice[0] = dice1;
	dice[1] = dice2;

	/* daca am dat dubla */
	if (dice1 == dice2)
	{
		/* se genereaza solutiile partile pentru 2 mutari */
		gen_partial_moves(sol,player,dice,tree);
		tree2 = tree;

		/* se genereaza solutiile finale pornind de la solutiile partiale
		 * generate anterior si de la celelalte doua mutari ramase */
		for (unsigned int i = 0; i < tree.size(); i++)
			gen_partial_moves(tree[i],player,dice,final_tree);

		/* daca nu s-au gasit mutari finale se returneaza cele intermediare */
		if (!final_tree.size() && tree2.size())
			return tree2;
	}
	
	/* daca este dubla se genereaza mutarile atat pentru perechea de zaruri
	 * initiala cat si pentru inversul ei;
	 * exemplu: daca am dar (5 3) genrez solutii pentru (5 3) si (3 5) */
	else
	{
		dice[0] = dice1;
		dice[1] = dice2;
	
		gen_partial_moves(sol,player,dice,final_tree);
	
		dice[0] = dice2;
		dice[1] = dice1;
	
		gen_partial_moves(sol,player,dice,final_tree);
	}
	
	return final_tree;	
}

/* functie care calculeaza scorul pentru o anumita tabla */
int calc_score( BGBoard board, Player player ) {
	int score = 0, sumB = 0, sumW = 0;
	
	/* se acorda o prioritate mare existenta cat mai multor porti */
	for (int i = 1; i <= SIZE; i++)
	{
		if (board.get_player(i) == player)
		{
			if (board.get_number(i) >= 2 )
			{
				score += 3000;
			}
		}

		/* se acorda punctje din ce in ce mai mari pentru
		 * piesele aflate cat mai aproape de propria casa */
		if (board.get_player(i) == White)
		{
			if (i >= 1 && i <= 6)
				sumW += 30;
			if (i >= 7 && i <= 12)
				sumW += 20;
			if (i >= 13 && i <=18)
				sumW += 10;
			if (i >= 19 && i <= 24)
				sumW += 1;
		}

		/* se acorda punctje din ce in ce mai mari pentru
		 * piesele aflate cat mai aproape de propria casa */
		if (board.get_player(i) == Black)
		{
			if (i >= 1 && i <= 6)
				sumB += 1;
			if (i >= 7 && i <= 12)
				sumB += 10;
			if (i >= 13 && i <=18)
				sumB += 20;
			if (i >= 19 && i <= 24)
				sumB += 30;
		}
	}

	if (player == White)
		score += sumW;
	
	if (player == Black)
		score += sumB;
	
	/* se cauta ca jucatorul advers sa fie scos pe bara */
	score -= board.get_inside(OPPONENT(player))*5;
	score += board.get_inside(player)*5;
	
	/* se cauta ca jucatorul aflat la mutare sa aiba
	 * cat mai multe piese in propria casa */
	score += board.get_bar(player)*5;
	score -= board.get_bar(OPPONENT(player))*5; 
	
	return score;
}

/* functie care calculeaza mutarea cea mai buna;
 * input: => tabla de joc
 * 		  => jucatorul ( White/Black )
 		  => perechea de zaruri primita de la server	
 * output: <= vector cu cea mai buna mutare */
Moves think( BGBoard board, Player player, int dice1, int dice2 ) {
	Tree tree;
	vector<int> move; 	
	int best_score = -1000, score = 0;
	
	/* se genereaza toate posibilitatile de mutare */
	tree = gen_all_moves(board,player,dice1,dice2);

	/* pentru fiecare solutie genrata de calculeaza  
	 * un scor si daca acesta este mai mare decat cea
	 * mai buna varinata pana la pasul anterior, atunci
	 * se actualizeaza 'best_score' */
	for (unsigned int i = 0; i < tree.size(); i++)
	{
		score = calc_score(tree[i].second,player);
		if (score > best_score)
		{
			best_score = score;
			move = tree[i].first;
		}
	}
	
	return move;
}

