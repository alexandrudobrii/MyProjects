#ifndef _SEGMENTATION_
	
	#define _SEGMENTATION_
	#include <vector>
	#include <string>
	#include "FlowGraph.h"

	using namespace std;

	/* variabile globale pentru datele citite din fisier */
	int lambda, treshold;
	int n, m;
	vector<int> mask_fg, mask_bg, image;

	const double PI = 3.141592;
	int v[4];
	
	void read_from_files();

	void parse_two (string, int *, int *);
	
	double miu_f();
	
	double miu_b();
	
	double sigma_f();
	
	double sigma_b();
	
	double f (int, int);
	
	double f (int, int, int, int);
	
	bool is_valid (int, int);
	
	void create_graph (FlowGraph&);
	
	void segment_image (vector<unsigned int>, vector<unsigned int>);

#endif