/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the Dijkstra shortest paths algorithm
Note: A=1, B=2, C=3,..., N=14
*/

#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;
using namespace std;
using namespace std::chrono;

int
main(int, char *[])
{
  typedef adjacency_list < listS, vecS, directedS,
    no_property, property < edge_weight_t, int > > graph_t;
  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef std::pair<int, int> Edge;

  const int num_nodes = 14;
  enum nodes { A, B, C, D, E, F, G, H, I, J, K, L, M, N};
  char name[] = "ABCDEFGHIJKLMN";
  /*Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };
  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };*/

  Edge edge_array[] = { Edge(A, C), Edge(A, D), Edge(B, E), Edge(C, B),
    Edge(C, E), Edge(C, J), Edge(D, E), Edge(D, G), Edge(D, H), Edge(E, F), Edge(F, M), Edge(G, D), Edge(H, G), Edge(H, I), Edge(I, J), Edge(I, L), Edge(J, C), Edge(J, F), Edge(K, L), Edge(L, I), Edge(L, K), Edge(L, N), Edge(M, N), Edge(N, M)
  };
  int weights[] = {8, 8, 7, 7, 8, 4, 1, 3, 2, 9, 4, 6, 3, 3, 2, 4, 10, 6, 6, 2, 8, 9, 6, 2};
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
  std::vector<vertex_descriptor> p(num_vertices(g));
  std::vector<int> d(num_vertices(g));
  vertex_descriptor s = vertex(A, g);

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  dijkstra_shortest_paths(g, s,
                          predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

  std::cout << "distances and parents:" << std::endl;
  graph_traits < graph_t >::vertex_iterator vi, vend;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
    std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
      endl;
  }
  std::cout << std::endl;

  std::ofstream dot_file("figs/dijkstra-eg.dot");

  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  graph_traits < graph_t >::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    graph_traits < graph_t >::edge_descriptor e = *ei;
    graph_traits < graph_t >::vertex_descriptor
      u = source(e, g), v = target(e, g);
    dot_file << name[u] << " -> " << name[v]
      << "[label=\"" << get(weightmap, e) << "\"";
    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"grey\"";
    dot_file << "]";
  }
  dot_file << "}";
  return EXIT_SUCCESS;
}
