/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the Prim minimum spanning tree algorithm
Note: A=1, B=2, C=3,..., N=14
*/

#include <boost/config.hpp>
#include <iostream>
#include <chrono>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace std::chrono;

int
main()
{
  using namespace boost;
  typedef adjacency_list < vecS, vecS, undirectedS,
    property<vertex_distance_t, int>, property < edge_weight_t, int > > Graph;
  typedef std::pair < int, int >E;
  const int num_nodes = 14;
  enum nodes { a, b, c, d, e, f, g, h, i, j, k, l, m, n};
  E edges[] = { E(a, c), E(a, d), E(b, e), E(c, b), E(c, e),
    E(c, j), E(d, e), E(d, g), E(d, h), E(e, f), E(f, m), E(g, d), E(h, g), E(h, i), E(i, j), E(i, l), E(j, c), E(j, f), E(k, l), E(l, i), E(l, k), E(l, n), E(m, n), E(n, m)
  };
  int weights[] = {8, 8, 7, 7, 8, 4, 1, 3, 2, 9, 4, 6, 3, 3, 2, 4, 10, 6, 6, 2, 8, 9, 6, 2};
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  Graph gr(num_nodes);
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, gr);
  for (std::size_t x = 0; x < sizeof(edges) / sizeof(E); ++x) {
    graph_traits<Graph>::edge_descriptor y; bool inserted;
    boost::tie(y, inserted) = add_edge(edges[j].first, edges[j].second, gr);
    weightmap[y] = weights[x];
  }
#else
  Graph gr(edges, edges + sizeof(edges) / sizeof(E), weights, num_nodes);
#endif
  std::vector < graph_traits < Graph >::vertex_descriptor >
    p(num_vertices(gr));

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  property_map<Graph, vertex_distance_t>::type distance = get(vertex_distance, gr);
  property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, gr);

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  prim_minimum_spanning_tree
    (gr, *vertices(gr).first, &p[0], distance, weightmap, indexmap,
     default_dijkstra_visitor());
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

#else
  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  prim_minimum_spanning_tree(gr, &p[0]);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

#endif

  for (std::size_t v = 0; v != p.size(); ++v)
    if (p[v] != v)
      std::cout << "parent[" << v << "] = " << p[v] << std::endl;
    else
      std::cout << "parent[" << v << "] = no parent" << std::endl;

  return EXIT_SUCCESS;
}
