#include "include.h"
#include "graph.h"

int** roy_marshall(int** g,size_t n)
{
  size_t u,v,w;

  int** res = xmalloc(n * sizeof(int*));
  for (u = 0; u < n; u++) {
    res[u] = xmalloc(n * sizeof(int));
    memcpy(res[u],g[u],n * sizeof(int));
  }
  for (w = 0; w < n; w++) {
    for (u = 0; u < n; u++) {
      for (v = 0; v < n; v++) {
        if (res[u][w] == 1 && res[w][v] == 1)
          res[u][v] = 1;
      }
    }
  }
  return res;
}
