#include "graph.h"
#include "roy_marshall.h"

Graph* grph_create(int** g,size_t n)
{ 
  Graph* res = grph_alloc();
  res->m_adj = g;
  res->m_transitive_closure = roy_marshall(g,n);
  res->sz = n;
  return res;
}

Inline Graph* grph_handler_edge(Graph* g,size_t a,size_t b,int handler)
{
  Graph* res = g;
  if (g->m_adj[a][b] == handler)
    return g;
  g->m_adj[a][b] = g->m_adj[b][a] = handler;
  g = grph_create(g->m_adj,g->sz);
  grph_free(res);
  return g;
}

Graph* grph_add_edge(Graph *g,size_t a,size_t b)
{ return grph_handler_edge(g,a,b,1); }

Graph* grph_remove_edge(Graph *g,size_t a,size_t b)
{ return grph_handler_edge(g,a,b,0); }

int grph_is_connected(CGraph *g)
{
  size_t i,j;
  for (i = 0; i < g->sz; i++) {
    for (j = i; j < g->sz; j++) {
      if (!g->m_transitive_closure[i][j])
        return 0;
    }
  }
  return 1;
}

void grph_compute_tc(Graph *g)
{
  if (g->m_transitive_closure != NULL)
    free_transitive_closure(g);
  g->m_transitive_closure = roy_marshall(g->m_adj,g->sz);
}

Graph* grph_generator(size_t nbVertex,size_t nbMaxEdges)
{
  size_t i,j,a,b,nbEdges = 0,nbEdgeToRemove = 0;
  Graph *res;
  int** m_adj = xmalloc(nbVertex * sizeof(int*));
  for (i = 0; i < nbVertex; i++)
    m_adj[i] = xcalloc(nbVertex,sizeof(int));

  res = grph_create(m_adj,nbVertex);

  while (nbEdges < nbMaxEdges) {
    a = rand()%nbVertex;
    b = rand()%nbVertex;
    if (edge_exists(res,a,b) || a == b)
      continue;
    res = grph_add_edge_wthout_tc(res,a,b);
    nbEdges++;
  }

  grph_compute_tc(res);

  for (i = 0; i < nbVertex; i++) {
    for (j = i+1; j < nbVertex; j++) {
      if (!vertex_are_connected(res,i,j)) {
        res = grph_add_edge(res,i,j);
        nbEdgeToRemove++;
      }
    }
  }
  while (nbEdgeToRemove) {
    a = rand()%nbVertex;
    for (i = 0; i < nbVertex; i++) {
      if (edge_exists(res,a,i) && a != i) {
        res = grph_remove_edge(res,a,i);
        if (!vertex_are_connected(res,a,i)) {
          res = grph_add_edge(res,a,i);
          continue;
        }
        if (--nbEdgeToRemove == 0)
          goto coda_grph_compute_tc;
      }
    }
  }

coda_grph_compute_tc:
  return res;
}
