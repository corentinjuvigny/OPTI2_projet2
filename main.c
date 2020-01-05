#include "graph.h"
#include "pl.h"

#define N 10
#define NBV 3

int main()
{
  size_t i,j,n = 4;
  const size_t values[] = {20,50,100}; //,400,600,1000};
  Graph *g;
  Graph **graphs[NBV], **trees[NBV];
  int** gr = (int**)malloc(n*sizeof(int*));
  for (i = 0; i < n; i++)
    gr[i] = (int*)malloc(n * sizeof(int));

  srand(time(NULL));

  /* I. Génération de graphes aléatoires */

  // 2)
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


  g = grph_create(gr,n);

  FREE_MADJ(g,i);
  grph_free(g);

  // 5)

  for (i = 0; i < NBV; i++) {
    graphs[i] = malloc(N * sizeof(*(graphs[i])));
    trees[i] = malloc(N * sizeof(*(trees[i])));
  }

  for (j = 0; j < NBV; j++) {
    for (i = 0; i < N; i++) {
      graphs[j][i] = grph_generator(values[j],grph_formula(values[j]));
    }
  }
  
  /* II. Résolution approchée (Heuristique) */

  // 2)
  
  for (j = 0; j < NBV; j++) {
    for (i = 0; i < N; i++) {
      trees[j][i] = mbvst_heuristic(graphs[j][i]);
      grph_compute_tc(trees[j][i]);
    }
  }

  /* III. Résolution exacte */
//  mbvst_pl(g);

  /* libération de la mémoire */

  for (j = 0; j < NBV; j++) {
    for (i = 0; i < N; i++) {
      FREE_MADJ(trees[j][i],n);
      FREE_MADJ(graphs[j][i],n);
      grph_free(trees[j][i]);
      grph_free(graphs[j][i]);
    }
    free(graphs[j]);
    free(trees[j]);
  }

  return 0;
}
