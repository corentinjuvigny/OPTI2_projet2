#include "graph.h"
#include "pl.h"

int main()
{
  size_t i,j,n = 4,m;
  int** gr = (int**)malloc(n*sizeof(int*));
  
  for (i = 0; i < n; i++)
    gr[i] = (int*)malloc(n * sizeof(int));

  gr[0][0] = 0;
  gr[0][1] = 1;
  gr[0][2] = 1;
  gr[0][3] = 0;
  gr[1][0] = 1;
  gr[1][1] = 0;
  gr[1][2] = 0;
  gr[1][3] = 1;
  gr[2][0] = 0;
  gr[2][1] = 0;
  gr[2][2] = 0;
  gr[2][3] = 1;
  gr[3][0] = 0;
  gr[3][1] = 1;
  gr[3][2] = 0;
  gr[3][3] = 0;

  srand(time(NULL));

  Graph* g = grph_create(gr,n);

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      printf("%d ",g->m_adj[i][j]);
    printf("\n");
  }
  printf("\n");
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      printf("%d ",g->m_transitive_closure[i][j]);
    printf("\n");
  }

  g = grph_add_edge(g,0,3);
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      printf("%d ",g->m_adj[i][j]);
    printf("\n");
  }
  printf("\n");

  g = grph_remove_edge(g,1,3);
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      printf("%d ",g->m_adj[i][j]);
    printf("\n");
  }
  printf("\n");

  g->m_adj[0][0] = 0;
  g->m_adj[0][1] = 1;
  g->m_adj[0][2] = 1;
  g->m_adj[0][3] = 1;
  g->m_adj[1][0] = 1;
  g->m_adj[1][1] = 0;
  g->m_adj[1][2] = 0;
  g->m_adj[1][3] = 0;
  g->m_adj[2][0] = 1;
  g->m_adj[2][1] = 0;
  g->m_adj[2][2] = 0;
  g->m_adj[2][3] = 0;
  g->m_adj[3][0] = 1;
  g->m_adj[3][1] = 0;
  g->m_adj[3][2] = 0;
  g->m_adj[3][3] = 0;

  printf("nbr connected componants = %ld, degree = %ld\n",grph_nbr_connected_componants(g,1),grph_degree_vertex(g,1));

  for (i = 0; i < n; i++)
    free(g->m_adj[i]);
  free(g->m_adj);

  grph_free(g);

  m = 20;
  g = grph_generator(m,grph_formula(m));

  int count=0;
  for (i = 0; i < m; i++)
    for (j = i; j < m; j++)
      if (g->m_adj[i][j]) count++;
  printf("%d\n",count);
  printf("is connected : %d\n",grph_is_connected(g));

  printf("%ld\n",grph_formula(m));

  Graph* t = mbvst_heuristic(g);

  grph_compute_tc(t);

  count = 0;
  for (i = 0; i < m; i++)
    for (j = i; j < m; j++)
      if (t->m_adj[i][j]) count++;
  printf("t is connected : %d and its size is %d\n",grph_is_connected(t),count);

  FREE_MADJ(t,i);

  grph_free(t);

  mbvst_pl(g);

  FREE_MADJ(g,i);

  grph_free(g);

  return 0;
}
