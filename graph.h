#ifndef __GRAPH__
#define __GRAPH__

#include "tools.h"
#include "list.h"
#include "fifo.h"

typedef struct _Graph Graph;
typedef const Graph CGraph;
struct _Graph {
  int**  m_adj;                 // Matrice d'adjacence
  int**  m_transitive_closure;  // Matrice d'adjacence de la fermeture transitive
  size_t sz;                    // Taille de m_adj
};

typedef struct _GraphMBVST MBVSTGraph;
typedef const MBVSTGraph CMBVSTGraph;
struct _GraphMBVST {
  Graph* grph; // Graphe
  list_t   edges; // liste des arêtes avec poids
  int*   vertex_type; // Tableau contenant le type de chacun des sommets
  void*  aomap;
};

typedef struct _MBVSTTree MBVSTTree;
typedef const MBVSTTree CMBVSTTree;
struct _MBVSTTree {
  Graph* grph;
  list_t edges;
};

typedef struct _EdgeTree MBVSTedge_t;
struct _EdgeTree {
  edge_t edge;
  int alpha;
  int omega;
};

#ifdef __cplusplus
extern "C" {
#endif

#define FREE_MADJ(g,i) for (i = 0; i < g->sz; i++)\
                          free(g->m_adj[i]);\
                       free(g->m_adj);

static void free_transitive_closure(Graph *g)
{
  size_t l;
  for (l = 0; l < g->sz; free(g->m_transitive_closure[l++]));
  xfree(g->m_transitive_closure);
}

/* Memory handlers */

#ifndef __cplusplus
Inline Graph* grph_alloc()
{ return xmalloc(sizeof(struct _Graph)); }
#else
Inline Graph* grph_alloc()
{ return (Graph*)xmalloc(sizeof(struct _Graph)); }
#endif

extern Graph* grph_create(int **g,size_t n);

extern Graph* grph_empty(size_t n);

extern Graph* grph_cpy(CGraph *g);

extern Graph* grph_cut_cpy(CGraph *g,size_t v);

Inline void grph_free(Graph *g) { free_transitive_closure(g); xfree(g); }

/* Usefull functions */

/* Add the edge between a and b in g */
extern Graph* grph_add_edge(Graph *g,size_t a,size_t b);

/* Add the edge without computing the transitive closure */
Inline Graph* grph_add_edge_wthout_tc(Graph *g,size_t a,size_t b)
{
  g->m_adj[a][b] = g->m_adj[b][a] = 1;
  return g;
}

/* Remove the edge between a and b in g */
extern Graph* grph_remove_edge(Graph *g,size_t a,size_t b);

extern void grph_compute_tc(Graph *g);

/* Return 1 if a and b linked in g, else 0 */
Inline int vertex_are_connected(CGraph *g,size_t a,size_t b)
{ return g->m_transitive_closure[a][b]; }

Inline int edge_exists(CGraph* g,size_t a,size_t b)
{ return g->m_adj[a][b]; }

/* Return 1 if g is connected, else 0 */
extern int grph_is_connected(CGraph *g);

/* Generate a connected graph G = (V,E) with |V| = nbVertex & |E| = nbMaxEdges */
Graph* grph_generator(size_t nbVertex,size_t nbMaxEdges);

Inline size_t grph_formula(size_t nbVertex)
{
  return (size_t)floor(nbVertex - 1 + 2 * 1.5 * ceil(sqrt((double)nbVertex)));
}

/* Return the number of connected componants in G - v */
size_t grph_nbr_connected_componants(CGraph *g,const size_t v);

size_t grph_degree_vertex(CGraph *g, const size_t k);

/* MBVSTGraph functions */

extern MBVSTGraph* MBVSTGraph_create(Graph *g);

Inline void MBVSTGraph_add_edge(MBVSTGraph *g,edge_t e)
{
  g->grph = grph_add_edge(g->grph,e.first,e.second);
  push(&g->edges,e);
}

Inline void MBVSTGraph_remove_edge(MBVSTGraph *g,edge_t e)
{
  g->grph = grph_remove_edge(g->grph,e.first,e.second);
  rm_l(&g->edges,e);
}

extern void MBVSTGraph_free(MBVSTGraph *g);

extern int MBVSTedge_cmp(const void *a, const void *b);

/* MBVSTTree functions */

extern MBVSTTree* MBVSTTree_create(const size_t size);

Inline void MBVSTTree_add_edge(MBVSTTree *t,edge_t e)
{
  t->grph = grph_add_edge(t->grph,e.first,e.second);
  push(&t->edges,e);
}

Inline void MBVSTTree_remove_edge(MBVSTTree *t,edge_t e)
{
  t->grph = grph_remove_edge(t->grph,e.first,e.second);
  rm_l(&t->edges,e);
}

Inline void MBVSTTree_free(MBVSTTree *t)
{ freel(t->edges); xfree(t); }

/* Return a cycle free tree graph */
extern Graph* mbvst_heuristic(Graph *g);

#ifdef __cplusplus
}
#endif

#endif
