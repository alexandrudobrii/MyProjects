#pragma once

#include "mpi.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>

using namespace std;
typedef vector<string> Messages;

#define MASTER 0
#define BROADCAST -1
#define INF 999999
#define NONE 0

void gen_routing_table (int **graph, int source, int detour[], int numtasks);
void split_buffer (string line, vector<int>& buffer);
void MPI_MyBcast (string message, int detour[], int numtasks);
void write (int rank, int detour[], int numtasks);

int **alloc(int numtasks);
int get_source (std::string message);
int get_destination (std::string message);
int string_to_int (string number);

std::string get_message (std::string message);
std::string int_to_string (int number);
std::vector<int> read_topology (int rank, int n, char *filename);
std::vector<int> read_messages (int rank, char *filename, Messages& message);
std::string create_message (int source, int destination, std::string info);
std::pair<int, int> get_count (std::vector<int> leader, std::vector<int> adjunct);

