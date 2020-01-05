#include "graph.h"
#include "roy_marshall.h"
#include "minimal_cut.hpp"

Graph* grph_create(int** g,size_t n)
{ 
  Graph* res = grph_alloc();
  res->m_adj = g;
  res->m_transitive_closure = roy_marshall(g,n);
  res->sz = n;
  return res;
}

Graph* grph_empty(size_t n)
{
  size_t i;
  int** m_adj = xmalloc(n * sizeof(int*));
  for (i = 0; i < n; i++)
    m_adj[i] = xcalloc(n,sizeof(int));
  return grph_create(m_adj,n);
}

Graph* grph_cpy(CGraph *g)
{
  size_t i,j;
  Graph *res;
  int** m_adj = xmalloc(g->sz * sizeof(int*));
  int** m_transitive_closure = xmalloc(g->sz * sizeof(int*));

  for (i = 0; i < g->sz; i++) {
    m_adj[i] = xcalloc(g->sz,sizeof(int));
    m_transitive_closure[i] = xcalloc(g->sz,sizeof(int));
    for (j = 0; j < g->sz; j++) {
      m_adj[i][j] = g->m_adj[i][j];
      m_transitive_closure[i][j] = g->m_transitive_closure[i][j];
    }
  }

  res = grph_alloc();

  res->m_adj = m_adj;
  res->m_transitive_closure = m_transitive_closure;
  res->sz = g->sz;
  
  return res;
}

Graph* grph_cut_cpy(CGraph *g,size_t v)
{
  size_t i,j;
  size_t sz = g->sz - 1;
  int** m_adj = malloc(sz * sizeof(int*));

  for (i = 0; i < v; i++) {
    m_adj[i] = malloc(sz * sizeof(int));
    for (j = 0; j < v; j++) {
      m_adj[i][j] = g->m_adj[i][j];
    }
    for (j = v+1; j < g->sz; j++) {
      m_adj[i][j-1] = g->m_adj[i][j];
    }
  }
  for (i = v+1; i < g->sz; i++) {
    m_adj[i-1] = malloc(sz * sizeof(int));
    for (j = 0; j < v; j++) {
      m_adj[i-1][j] = g->m_adj[i][j];
    }
    for (j = v+1; j < g->sz; j++) {
      m_adj[i-1][j-1] = g->m_adj[i][j];
    }
  }

  return grph_create(m_adj,sz);
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

  res = grph_empty(nbVertex);

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

static void grph_connected_componant(CGraph *g,char *vertex_marked,const size_t v)
{
  size_t s,t;
  fifo_t f = fifo_create(g->sz,sizeof(v));

  fifo_add(f,&v);
  vertex_marked[v] = 1;
  while (!fifo_is_empty(f)) {
    fifo_get(f,&s);
    for (t = 0; t < g->sz; t++) {
      if (g->m_adj[s][t] && !vertex_marked[t]) {
        fifo_add(f,&t);
        vertex_marked[t] = 1;
      }
    }
  }
  fifo_free(f);
}

static int all_vertex_are_marked(size_t* v,const char* vertex_marked,const size_t len)
{
  size_t i;
  for (i = 0; i < len; i++)
    if ( !vertex_marked[i] ) {
      *v = i;
      return 0;
    }
  return 1;
}

size_t grph_nbr_connected_componants(CGraph *cg,const size_t v)
{
  if ( cg->sz == 1 ) return 0;
  size_t res = 0, i, k;
  Graph* g = grph_cut_cpy(cg,v);
  char vertex_marked[g->sz];

  for (i = 0; i < g->sz; i++)
    vertex_marked[i] = 0;

  k = v == 0 ? 1 : 0;
  do {
    grph_connected_componant(g,vertex_marked,k);
    res++;
  } while (!all_vertex_are_marked(&k,vertex_marked,g->sz));

  FREE_MADJ(g,i);
  grph_free(g);

  return res;
}

size_t grph_degree_vertex(CGraph *g, const size_t k)
{
  size_t l, res = 0;
  for (l = 0; l < g->sz; l++)
    if (g->m_adj[k][l])
      res++;
  return res;
}

static int grph_vertex_type(CGraph *g,const size_t k)
{
  size_t cg, dg = grph_degree_vertex(g,k);
  
  switch ( dg ) {
    case 0 :
      fprintf(stderr,"Error : degree of %ld can't be null\n",k);
      return -1;
    case 1:
      return 0;
    case 2:
      cg = grph_nbr_connected_componants(g,k);
      if ( cg == 1 ) return 1;
      if ( cg == 2 ) return 0;
      fprintf(stderr,"Error : impossible case : k = %ld, cg = %ld and dg = %ld)\n",k,cg,dg);
      break;
    default:
      cg = grph_nbr_connected_componants(g,k);
      if ( cg <= 2 ) return 2;
  }
  return 3;
}

int MBVSTedge_cmp(const void *a, const void *b)
{
  const MBVSTedge_t *e1 = a;
  const MBVSTedge_t *e2 = b;
  if ( e1->edge.first < e2->edge.first )
    return -1;
  if ( e1->edge.first > e2->edge.first )
    return 1;
  if ( e1->edge.second < e2->edge.second )
    return -1;
  if ( e1->edge.second > e2->edge.second )
    return 1;
  return 0;
}

MBVSTGraph* MBVSTGraph_create(Graph *g)
{
  size_t k,l;
  MBVSTGraph* res = malloc(sizeof(*res));
  MBVSTedge_t* te;

  res->grph = g;
  res->edges = initl();
  res->aomap = NULL;
  res->vertex_type = malloc(g->sz * sizeof(int));
  for (k = 0; k < g->sz; k++) {
    for (l = k; l < g->sz; l++) {
      if ( g->m_adj[k][l] == 1 ) {
        edge_t e = {k,l};
        push(&res->edges,e);
        te = malloc(sizeof(*te));
        te->edge = e;
        te->alpha = te->omega = 1;
        tsearch(te,&res->aomap,MBVSTedge_cmp);
      }
    }
    res->vertex_type[k] = grph_vertex_type(g,k);
  }

  return res;
}

void MBVSTGraph_free(MBVSTGraph *g)
{ 
  freel(g->edges);
  tdestroy(g->aomap,free);
  xfree(g->vertex_type);
  xfree(g);
}

MBVSTTree* MBVSTTree_create(const size_t size)
{
  MBVSTTree* res = malloc(sizeof(*res));
  res->grph = grph_empty(size);
  res->edges = initl();
  return res;
}

static edge_t MBVST_separate(MBVSTTree *t,list_t l)
{
  long rnd;
  list_t current = l;

  while (current != NULL) {
    if ( grph_degree_vertex(t->grph,current->s.first) == 1 
        && grph_degree_vertex(t->grph,current->s.second) == 1 )
      return current->s;
    current = current->next;
  }
  rnd = rand()%nbrOfElement(l);
  
  return lget(l,rnd);
}

static void MBVST_saturate(MBVSTTree *t,MBVSTGraph *g,const size_t v)
{
  size_t m,w;
  char vertex_marked[g->grph->sz];
  list_t l = list_diff(g->edges,t->edges);
  list_t lsave = l;
  MBVSTedge_t **r;

  while (l != NULL) {
    if ( l->s.first == v )
      w = l->s.second;
    else {
      if ( l->s.second == v ) {
        w = l->s.first;
      } else goto next_step;
    }
    for (m = 0; m < g->grph->sz; m++)
      vertex_marked[m] = 0;
    grph_connected_componant(t->grph,vertex_marked,v);
    if ( vertex_marked[w] == 0 && g->vertex_type[w] != 2 ) {
      MBVSTTree_add_edge(t,l->s);
      MBVSTedge_t e = {l->s,0,0};
      r = tfind(&e,&g->aomap,MBVSTedge_cmp);
      if ( r == NULL ) {
        goto next_step;
      }
      (*r)->alpha = INF;
    }
next_step:
    l = l->next;
  }
  
  freel(lsave);
}

static void MBVST_reduce_omega(const void *nodep, VISIT which, void* closure)
{
  MBVSTedge_t *e = *(MBVSTedge_t**)nodep;
  size_t v = *(size_t*)closure;
  switch (which) {
    case preorder:
      break;
    case endorder:
      break;
    case postorder:
    case leaf:
      if ( e->edge.first == v || e->edge.second == v ) {
        e->omega -= e->omega < 3 ? e->omega : 3;
      }
      break;
    default:
      break;
  }
}

static void MBVST_increase_omega(const void *nodep, VISIT which, void* closure)
{
  MBVSTedge_t *e = *(MBVSTedge_t**)nodep;
  size_t v = *(size_t*)closure;
  switch (which) {
    case preorder:
      break;
    case endorder:
      break;
    case postorder:
    case leaf:
      if ( e->edge.first == v || e->edge.second == v ) {
        e->omega += 1;
      }
      break;
    default:
      break;
  }
}

static void MBVST_change_type(MBVSTTree *t,MBVSTGraph *g,size_t v)
{
  if ( grph_degree_vertex(t->grph,v) > 2 ) {
    g->vertex_type[v] = 3;
    //diminuer de 3 omega les arêtes incidentes à v
    twalk_r(g->aomap,MBVST_reduce_omega,&v);
    MBVST_saturate(t,g,v);
  }
}

Graph* mbvst_heuristic(Graph *g)
{
  size_t v;
  int omega;
  edge_t e;
  Graph *res;
  MBVSTGraph *gp;
  MBVSTTree *t;
  list_t min_cut_l;
  MBVSTedge_t **r, emap;

  t = MBVSTTree_create(g->sz);
  gp = MBVSTGraph_create(g);
 
  for (v = 0; v < g->sz; v++) 
    if ( gp->vertex_type[v] == 3 )
      MBVST_saturate(t,gp,v);

  while (nbrOfElement(t->edges) < g->sz - 1) {
    min_cut_l = minimal_cut(gp,&omega);
    if ( omega == 3 ) {
      e = MBVST_separate(t,min_cut_l);
    } else {
      e = lget(min_cut_l,rand()%nbrOfElement(min_cut_l));
    }
    MBVSTTree_add_edge(t,e);
    if ( gp->vertex_type[e.first] == 2 ) {
      twalk_r(gp->aomap,MBVST_increase_omega,&(e.first));
      MBVST_change_type(t,gp,e.first);
    }
    if ( gp->vertex_type[e.second] == 2 ) {
      twalk_r(gp->aomap,MBVST_increase_omega,&(e.second));
      MBVST_change_type(t,gp,e.first);
    }
    emap.edge = e;
    r = tfind(&emap,&gp->aomap,MBVSTedge_cmp);
    if ( r == NULL ) {
      freel(min_cut_l);
      goto TERMINATE;
    }
    (*r)->alpha = INF;
    freel(min_cut_l);
  }

TERMINATE:
  res = t->grph;
  MBVSTGraph_free(gp);
  MBVSTTree_free(t);

  return res;
}
