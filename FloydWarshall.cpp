/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the Floyd Warshall all pairs shortest paths algorithm
*/

#include <iostream>
#include <chrono>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

using namespace std;
using namespace std::chrono;

// type for weight/distance on each edge
typedef double t_weight;

// define the graph type
typedef boost::property<boost::edge_weight_t, t_weight> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

// Declare a matrix type and its corresponding property map that
// will contain the distances between each pair of vertices.
typedef boost::exterior_vertex_property<Graph, t_weight> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;


int main()
{
  Graph g;

  const int num_edges = 23;

  // define edges
  int edges[] = { 1,     3,
                  1,     4,
                  3,     2,
                  3,     5,
                  3,     10,
                  2,     5,
                  5,     6,
                  10,     3,
                  6,     13,
                  13,     14,
                  4,     5,
                  4,     7,
                  4,     8,
                  7,     4,
                  8,     7,
                  8,     9,
                  9,     10,
                  9,     12,
                  12,    9,
                  12,    11,
                  11,    12,
                  12,    14,
                  14,    13,
                };

  // define the weight on edges
  t_weight weight[] = { 8, 8, 7, 7, 8, 4, 1, 3, 2, 9, 4, 6, 3, 3, 2, 4, 10, 6, 6, 2, 8, 9, 6, 2};

  // iterate over all edges and insert them in the graph
  for (std::size_t k = 0; k < num_edges; ++k)
    boost::add_edge(edges[k*2]-1, edges[k*2+1]-1, weight[k], g);

  WeightMap weight_pmap = boost::get(boost::edge_weight, g);

  // set the distance matrix to receive the floyd warshall output
  DistanceMatrix distances(num_vertices(g));
  DistanceMatrixMap dm(distances, g);

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  bool valid = floyd_warshall_all_pairs_shortest_paths(g, dm, boost::weight_map(weight_pmap));
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;


  // check if there no negative cycles
  if (!valid) {
    std::cerr << "Error - Negative cycle in matrix" << std::endl;
    return -1;
  }

  // print upper triangular part of the distance matrix
  std::cout << "Distance matrix: " << std::endl;
  for (std::size_t i = 0; i < num_vertices(g); ++i) {
    for (std::size_t j = i; j < num_vertices(g); ++j) {
      std::cout << "From vertex " << i+1 << " to " << j+1 << " : ";
      if(distances[i][j] == std::numeric_limits<t_weight>::max())
        std::cout << "inf" << std::endl;
      else
        std::cout << distances[i][j] << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
