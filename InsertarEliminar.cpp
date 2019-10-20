/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify an undirected graph, along with deleting edges/vertices
*/

#include <chrono>
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace std::chrono;

typedef boost::property<boost::edge_weight_t, double> EdgeWeight;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeight> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator edge_iterator;

int main(int, char*[])
{
  char name[] = "1234567891011121314";

  UndirectedGraph g;

  /*auto start = high_resolution_clock::now();
  boost::add_vertex(g);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution for adding a vertex: " << duration.count() << " microseconds." << endl;*/

  /*auto start = high_resolution_clock::now();
  boost::add_edge(1, 3, 8, g);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution for inserting an edge: " << duration.count() << " microseconds." << endl;*/

  boost::add_edge(1, 3, 8, g);
  boost::add_edge(1, 4, 8, g);
  boost::add_edge(3, 2, 7, g);
  boost::add_edge(3, 5, 8, g);
  boost::add_edge(3, 10, 4, g);
  boost::add_edge(2, 5, 7, g);
  boost::add_edge(5, 6, 9, g);
  boost::add_edge(10, 3, 10, g);
  boost::add_edge(6, 13, 4, g);
  boost::add_edge(13, 14, 6, g);
  boost::add_edge(4, 5, 1, g);
  boost::add_edge(4, 7, 3, g);
  boost::add_edge(4, 8, 2, g);
  boost::add_edge(7, 4, 6, g);
  boost::add_edge(8, 7, 3, g);
  boost::add_edge(8, 9, 3, g);
  boost::add_edge(9, 10, 2, g);
  boost::add_edge(9, 12, 4, g);
  boost::add_edge(12, 9, 2, g);
  boost::add_edge(12, 11, 8, g);
  boost::add_edge(11, 12, 6, g);
  boost::add_edge(12, 14, 9, g);
  boost::add_edge(14, 13, 2, g);

  auto start = high_resolution_clock::now();
  boost::remove_edge(14, 13, g);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution for removing an edge: " << duration.count() << " microseconds." << endl;

  /*auto start = high_resolution_clock::now();
  boost::remove_vertex(15, g);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution for removing a vertex: " << duration.count() << " microseconds." << endl;*/

  boost::property_map<UndirectedGraph, boost::edge_weight_t>::type EdgeWeightMap = get(boost::edge_weight_t(), g);

  pair<edge_iterator, edge_iterator> edgePair;

  return 0;
}//Close main
