#ifndef __FLOW_GRAPH__
#define __FLOW_GRAPH__

#pragma once

#include <iostream>
#include <fstream>

typedef std::vector< std::vector <int> > Edges;

struct FlowGraph {
 public:
  unsigned int n;
  double **capacity;
  Edges edges;

 public:
  FlowGraph() : n(0), capacity(NULL) { }

  ~FlowGraph() {
    if (capacity != NULL) {
      for (unsigned int i = 0; i < n; ++i) {
        delete[] capacity[i];
      }
      delete[] capacity;
    }
  }

  /** \brief Functie care intoarce numarul de noduri din graf.
   * \return Numarul de noduri din graf.
   */
  inline unsigned int size() {
    return n;
  }

  /** \brief Functie care permite accesul la graf ca la o matrice de adiacenta.
   * \param index Un nod in graf.
   * \return Un pointer catre linia din matricea de adiacenta a nodului index.
   */
  inline double* operator[] (int index) {
    return capacity[index];
  }

  /** \brief Functie care spune daca muchia (u, v) din graf este sau nu
   * saturata.
   * \return <b>True</b> daca muchia este saturata, sau <b>False</b> altfel.
   */
  inline bool is_saturated(unsigned int u, unsigned int v) {
    return capacity[u][v] == 0;
  }
};

  std::vector<unsigned int> bfs(FlowGraph&, unsigned int, unsigned int);

  double saturate_path(FlowGraph&, std::vector<unsigned int>&);

  double maximum_flow(FlowGraph&, unsigned int, unsigned int);

  void min_cut(FlowGraph&, std::vector<unsigned int>&,
                             std::vector<unsigned int>&, unsigned int);

#endif

