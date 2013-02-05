#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "FlowGraph.h"

#define NONE -1

std::vector<unsigned int> bfs(FlowGraph& graph,
                              unsigned int source,
                              unsigned int sink)
{
  /* Ne vom folosi de vectorul de parinti pentru a spune daca un nod a fost
   * adaugat sau nu in coada. */
  std::vector<unsigned int> parent(graph.size(), NONE);

  std::deque<unsigned int> q;
  q.push_back(source);
  ;
  while ((int)parent[sink] == NONE && q.size() > 0) {
    unsigned int node = q.front();
    q.pop_front();

    for (unsigned int i = 0; i < graph.edges[node].size(); ++i) {
      if ((int)parent[graph.edges[node][i]] == NONE 
                              && graph[node][graph.edges[node][i]] > 0) {
        parent[graph.edges[node][i]] = node;
        if (graph.edges[node][i] != (int)sink) {
          q.push_back(graph.edges[node][i]);
        } else {
          q.clear();
          break;
        }
      }
    }
  }

  /* Daca nu s-a atins destinatia, atunci nu mai exista drumuri de crestere. */
  if ((int)parent[sink] == NONE) {
    return std::vector<unsigned int>();
  }

  /* Reconstituim drumul de la destinatie spre sursa. */
  std::vector<unsigned int> returnValue;
  for (unsigned int node = sink; true; node = parent[node]) {
    returnValue.push_back(node);
    if (node == source) {
      break;
    }
  }

  /* Inversam drumul pentru a incepe cu sursa si a se termina cu destinatia. */
  std::reverse(returnValue.begin(), returnValue.end());

  return returnValue;
}

double saturate_path(FlowGraph& graph,
                           std::vector<unsigned int>& path)
{
  /* Niciodata nu ar trebui sa se intample asta pentru ca sursa si destinatia
   * sunt noduri distincte si cel putin unul dintre ele se afla in path. */
  if (path.size() < 2) {
    return 0;
  }

  /* Determinam fluxul maxim prin drum. */
  double flow = graph[path[0]][path[1]];
  for (unsigned int i = 0; i < path.size() - 1; ++i) {
    unsigned int u = path[i];
    unsigned int v = path[i + 1];
    flow = (flow < graph[u][v] ? flow : graph[u][v]);
  }

  /* Si il saturam in graf. */
  for (unsigned int i = 0; i < path.size() - 1; ++i) {
    unsigned int u = path[i];
    unsigned int v = path[i + 1];
    graph[u][v] -= flow;
    graph[v][u] += flow;
  }

  /* Raportam fluxul cu care am saturat graful. */
  return flow;
}

double maximum_flow(FlowGraph& graph, unsigned int source, unsigned int sink) {
  double returnValue = 0;

  /* Vom incerca in mod repetat sa determinam drumuri de crestere folosind
   * BFS si sa le saturam pana cand nu mai putem determina un astfel de drum in
   * graf. */
  while (true) {
    /* Determina drum de crestere. */
    std::vector<unsigned int> saturation_path = bfs(graph, source, sink);

    /* Daca nu exista drum de crestere, atunci intoarce fluxul maxim gasit. */
    if (saturation_path.size() == 0) {
      break;
    }

    /* Altfel satureaza drumul. */
    returnValue += saturate_path(graph, saturation_path);
  }

  return returnValue;
}

void min_cut(FlowGraph& graph,
             std::vector<unsigned int>& source_set,
             std::vector<unsigned int>& sink_set,
             unsigned int source)
{
  /* Facem o parcurgere BFS din nodul sursa si punem nodurile atinse de
   * parcurgere in source_set. Toate celelalte noduri se vor afla in
   * sink_set. */
  std::vector<bool> in_queue(graph.size(), false);
  std::deque<unsigned int> q;
  q.push_back(source);
  in_queue[source] = true;

  /* Rulam BFS din sursa si marcam nodurile atinse. */
  while (q.size()) {
    unsigned int node = q.front();
    q.pop_front();

    for (unsigned int i = 0; i < graph.edges[node].size(); ++i) {
      if (in_queue[graph.edges[node][i]] == false 
                            && graph[node][graph.edges[node][i]] > 0) {
        in_queue[graph.edges[node][i]] = true;
        q.push_back(graph.edges[node][i]);
      }
    }
  }

  /* Traversam inca odata si impartim nodurile intre cele doua multimi. */
  for (unsigned int i = 0; i < graph.size(); ++i) {
    if (in_queue[i]) {
      source_set.push_back(i);
    } else {
      sink_set.push_back(i);
    }
  }
}



