/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the Kruskal shortest paths algorithm
*/

#include <chrono>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;

int main()
{
  using namespace boost;
  typedef adjacency_list < vecS, vecS, undirectedS,
    no_property, property < edge_weight_t, int > > Graph;
  typedef graph_traits < Graph >::edge_descriptor Edge;
  typedef std::pair<int, int> E;

  const int num_nodes = 14;
  E edge_array[] = { E(1, 3), E(1, 4), E(2, 5), E(3, 2),
    E(3, 5), E(3, 10), E(4, 5), E(4, 7), E(4, 8), E(5, 6), E(6, 13), E(7, 4), E(8, 7), E(8, 9), E(9, 10), E(9, 12), E(10, 3), E(10, 6), E(11, 12), E(12, 9), E(12, 11), E(12, 14), E(13, 14), E(14, 13)
  };
  int weights[] = {8, 8, 7, 7, 8, 4, 1, 3, 2, 9, 4, 6, 3, 3, 2, 4, 10, 6, 6, 2, 8, 9, 6, 2};
  std::size_t num_edges = sizeof(edge_array) / sizeof(E);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  Graph g(num_nodes);
  property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
  for (std::size_t j = 0; j < num_edges; ++j) {
    Edge e; bool inserted;
    boost::tie(e, inserted) = add_edge(edge_array[j].first, edge_array[j].second, g);
    weightmap[e] = weights[j];
  }
#else
  Graph g(edge_array, edge_array + num_edges, weights, num_nodes);
#endif
  property_map < Graph, edge_weight_t >::type weight = get(edge_weight, g);
  std::vector < Edge > spanning_tree;

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

  kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));

  std::cout << "Print the edges in the MST:" << std::endl;
  for (std::vector < Edge >::iterator ei = spanning_tree.begin();
       ei != spanning_tree.end(); ++ei) {
    std::cout << source(*ei, g) << " <--> " << target(*ei, g)
      << " with weight of " << weight[*ei]
      << std::endl;
  }

  std::ofstream fout("figs/kruskal-eg.dot");
  fout << "graph A {\n"
    << " rankdir=LR\n"
    << " size=\"3,3\"\n"
    << " ratio=\"filled\"\n"
    << " edge[style=\"bold\"]\n" << " node[shape=\"circle\"]\n";
  graph_traits<Graph>::edge_iterator eiter, eiter_end;
  for (boost::tie(eiter, eiter_end) = edges(g); eiter != eiter_end; ++eiter) {
    fout << source(*eiter, g) << " -- " << target(*eiter, g);
    if (std::find(spanning_tree.begin(), spanning_tree.end(), *eiter)
        != spanning_tree.end())
      fout << "[color=\"black\", label=\"" << get(edge_weight, g, *eiter)
           << "\"];\n";
    else
      fout << "[color=\"gray\", label=\"" << get(edge_weight, g, *eiter)
           << "\"];\n";
  }
  fout << "}\n";
  return EXIT_SUCCESS;
}
