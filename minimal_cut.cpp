#include "minimal_cut.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/typeof/typeof.hpp>
#include <search.h>

#ifdef __cplusplus
extern "C" {
#endif

list_t minimal_cut(MBVSTGraph *g,int *omega)
{
  using namespace std;

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property, boost::property<boost::edge_weight_t, int> > undirected_graph;
  typedef boost::property_map<undirected_graph, boost::edge_weight_t>::type weight_map_type;
  typedef boost::property_traits<weight_map_type>::value_type weight_type;

  list_t l = g->edges, res = initl();
  size_t i = 0, n = nbrOfElement(l);

  edge_t* edges = new edge_t[n];
  weight_type* ws = new weight_type[n];
  int* ow = new int[n];

  while (l != NULL) {
    edges[i] = l->s;
    MBVSTedge_t **e = (MBVSTedge_t**)tfind(edges+i,&g->aomap,MBVSTedge_cmp);
    if (e == NULL) {
      cerr << "Error edge " << i << " : (" << edges[i].first << "," << edges[i].second
           << ") not found in tree." << endl;
      exit(1);
    }
    ws[i] = (*e)->alpha;
    ow[i] = (*e)->omega;
    l = l->next;
    i++;
  }

  undirected_graph bg(edges,edges+n,ws,g->grph->sz,n);

  BOOST_AUTO(parities,boost::make_one_bit_color_map(boost::num_vertices(bg),get(boost::vertex_index,bg)));
  
  boost::stoer_wagner_min_cut(bg,get(boost::edge_weight,bg),boost::parity_map(parities));

  *omega = INF;

  for (i = 0; i < n; i++) {
    if (get(parities,edges[i].first) != get(parities,edges[i].second)) {
      if (ow[i] > *omega)
        continue;
      if (ow[i] < *omega) {
        *omega = ow[i];
        freel(res);
        res = NULL;
      }
      push(&res,edges[i]);
    }
  }

  delete[] ow;
  delete[] ws;
  delete[] edges;

  return res;
}

#ifdef __cplusplus
}
#endif
