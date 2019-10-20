/*
Author: Sergio Hernandez Castillo
Date: 10/10/2019
Description: A program that implements the boost library to make and modify a graph and use the BFS algorithm
Note: A=1, B=2, C=3,..., N=14
*/

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>
#include <chrono>
#include <iostream>

using namespace boost;
using namespace std;
using namespace std::chrono;

template < typename TimeMap > class bfs_time_visitor:public default_bfs_visitor {
  typedef typename property_traits < TimeMap >::value_type T;
public:
  bfs_time_visitor(TimeMap tmap, T & t):m_timemap(tmap), m_time(t) { }
  template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
  {
    put(m_timemap, u, m_time++);
  }
  TimeMap m_timemap;
  T & m_time;
};


int
main()
{
  using namespace boost;
  // Select the graph type we wish to use
  typedef adjacency_list < vecS, vecS, undirectedS > graph_t;
  // Set up the vertex IDs and names
  enum { a, b, c, d, e, f, g, h, i, j, k, l, m, n, N};
  const char *name = "abcdefghijklmn";
  // Specify the edges in the graph
  typedef std::pair < int, int >E;
  E edge_array[] = { E(a, c), E(a, d), E(b, e), E(c, b), E(c, e),
    E(c, j), E(d, e), E(d, g), E(d, h), E(e, f), E(f, m), E(g, d), E(h, g), E(h, i), E(i, j), E(i, l), E(j, c), E(j, f), E(k, l), E(l, i), E(l, k), E(l, n), E(m, n), E(n, m)
  };

  // Create the graph object
  const int n_edges = sizeof(edge_array) / sizeof(E);
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
  // VC++ has trouble with the edge iterator constructor
  graph_t gr(N);
  for (std::size_t j = 0; j < n_edges; ++j)
    add_edge(edge_array[j].first, edge_array[j].second, gr);
#else
  typedef graph_traits<graph_t>::vertices_size_type v_size_t;
  graph_t gr(edge_array, edge_array + n_edges, v_size_t(N));
#endif

  // Typedefs
  typedef graph_traits < graph_t >::vertices_size_type Size;

  // a vector to hold the discover time property for each vertex
  std::vector < Size > dtime(num_vertices(gr));
  typedef
    iterator_property_map<std::vector<Size>::iterator,
                          property_map<graph_t, vertex_index_t>::const_type>
    dtime_pm_type;
  dtime_pm_type dtime_pm(dtime.begin(), get(vertex_index, gr));

  Size time = 0;
  bfs_time_visitor < dtime_pm_type >vis(dtime_pm, time);

  auto start = high_resolution_clock::now();
  // find all pairs shortest paths
  breadth_first_search(gr, vertex(a, gr), visitor(vis));
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "\nTime of execution: " << duration.count() << " microseconds." << endl;

  // Use std::sort to order the vertices by their discover time
  std::vector<graph_traits<graph_t>::vertices_size_type > discover_order(N);
  integer_range < int >range(0, N);
  std::copy(range.begin(), range.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp < dtime_pm_type, std::less < Size > >(dtime_pm));

  std::cout << "order of discovery: ";
  for (int i = 0; i < N; ++i)
    std::cout << name[discover_order[i]] << " ";
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
