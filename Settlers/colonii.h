#pragma once
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

#define ITERATIONS 500
#define MAX_SIZE 100
#define MAXIM 99999

int resurse[MAX_SIZE][MAX_SIZE]; 
int pret[MAX_SIZE][MAX_SIZE]; 
int buget[MAX_SIZE][MAX_SIZE];

int resurse_urm[MAX_SIZE][MAX_SIZE]; 
int pret_urm[MAX_SIZE][MAX_SIZE]; 
int buget_urm[MAX_SIZE][MAX_SIZE];

int n, pmin, pmax, iterations;
int colA[ITERATIONS], colB[ITERATIONS], pretA[ITERATIONS], pretB[ITERATIONS];
