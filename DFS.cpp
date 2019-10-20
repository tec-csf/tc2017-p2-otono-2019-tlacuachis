/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the DFS algorithm
Note: A=1, B=2, C=3,..., N=14
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/range/irange.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <chrono>
#include <iostream>

using namespace boost;
using namespace std;
using namespace std::chrono;

template < typename TimeMap > class dfs_time_visitor:public default_dfs_visitor {
  typedef typename property_traits < TimeMap >::value_type T;
public:
  dfs_time_visitor(TimeMap dmap, TimeMap fmap, T & t)
:  m_dtimemap(dmap), m_ftimemap(fmap), m_time(t) {
  }
  template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
  {
    put(m_dtimemap, u, m_time++);
  }
  template < typename Vertex, typename Graph >
    void finish_vertex(Vertex u, const Graph & g) const
  {
    put(m_ftimemap, u, m_time++);
  }
  TimeMap m_dtimemap;
  TimeMap m_ftimemap;
  T & m_time;
};

int
main()
{
  // Select the graph type we wish to use
  typedef adjacency_list < vecS, vecS, directedS > graph_t;
  typedef graph_traits < graph_t >::vertices_size_type size_type;
  // Set up the vertex names
  enum { a, b, c, d, e, f, g, h, i, j, k, l, m, n, N};
  char name[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n' };
  // Specify the edges in the graph
  typedef std::pair < int, int >E;
  E edge_array[] = { E(a, c), E(a, d), E(b, e), E(c, b), E(c, e),
    E(c, j), E(d, e), E(d, g), E(d, h), E(e, f), E(f, m), E(g, d), E(h, g), E(h, i), E(i, j), E(i, l), E(j, c), E(j, f), E(k, l), E(l, i), E(l, k), E(l, n), E(m, n), E(n, m)
  };
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  graph_t gr(N);
  for (std::size_t z = 0; z < sizeof(edge_array) / sizeof(E); ++z)
    add_edge(edge_array[j].first, edge_array[j].second, gr);
#else
  graph_t gr(edge_array, edge_array + sizeof(edge_array) / sizeof(E), N);
#endif

  // discover time and finish time properties
  std::vector < size_type > dtime(num_vertices(gr));
  std::vector < size_type > ftime(num_vertices(gr));
  typedef
    iterator_property_map<std::vector<size_type>::iterator,
                          property_map<graph_t, vertex_index_t>::const_type>
    time_pm_type;
  time_pm_type dtime_pm(dtime.begin(), get(vertex_index, gr));
  time_pm_type ftime_pm(ftime.begin(), get(vertex_index, gr));
  size_type t = 0;
  dfs_time_visitor < time_pm_type >vis(dtime_pm, ftime_pm, t);

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  depth_first_search(gr, visitor(vis));
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

  /* use std::sort to order the vertices by their discover time
  std::vector < size_type > discover_order(N);
  integer_range < size_type > r(0, N);
  std::copy(r.begin(), r.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp < time_pm_type, std::less < size_type > >(dtime_pm));
  std::cout << "order of discovery: ";
  int y;
  for (y = 0; y < N; ++y)
    std::cout << name[discover_order[i]] << " ";

  std::vector < size_type > finish_order(N);
  std::copy(r.begin(), r.end(), finish_order.begin());
  std::sort(finish_order.begin(), finish_order.end(),
            indirect_cmp < time_pm_type, std::less < size_type > >(ftime_pm));
  std::cout << std::endl << "order of finish: ";
  for (y = 0; y < N; ++y)
    std::cout << name[finish_order[i]] << " ";
  std::cout << std::endl;*/

  return EXIT_SUCCESS;
}
