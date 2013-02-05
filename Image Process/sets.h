#pragma once
#include <cstdio>
#include <cstdlib>
#include <fstream>  
#include <vector>
#include <cmath>

#define MASTER 0
#define MANDELBROT 0
#define JULIA 1
#define NUM_COLORS 256

double x_min, x_max, y_min, y_max;
double a, b;
double resolution;
int max_steps, set;
int width, height;

void read_from_file (const char *filename);
void print_set (int **pgm, const char *filename);
void gen_set (double, double, double, double);
void gen_mandelbrot_set (double, double, double, double);
void gen_julia_set (double, double, double, double);

void MPI_Free_Matrix(int **array, int n);
void MPI_Send_Matrix (int **buffer, int n, int m);
int **MPI_Recv_Matrix (int n, int m, int source);
int **MPI_Alloc_Matrix(int n, int m);
