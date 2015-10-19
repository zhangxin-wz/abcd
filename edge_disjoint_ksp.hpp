#ifndef BOOST_GRAPH_EDGE_DISJOINT_KSP
#define BOOST_GRAPH_EDGE_DISJOINT_KSP

#include <map>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

namespace boost {

namespace detail {

  template <typename Graph, class Weight>
  void edge_disjoint_ksp(const Graph& g,
                         typename graph_traits<Graph>::vertex_descriptor s,
                         typename graph_traits<Graph>::vertex_descriptor t, 
                         Weight w)
  {
    typedef typename graph_traits<Graph>::edge_descriptor edge;

    // Fill in the maps.
    std::map<edge, edge> e2e;
    std::map<edge, int> e2c;
    std::map<edge, int> e2r;
    std::map<edge, int> e2w;

    typename graph_traits<Graph>::edge_iterator ei, ee;
    for (tie(ei, ee) = edges(g); ei != ee; ++ei)
      {
        // Normal edge.
        const edge &ne = *ei;
        // Reverse edge.
        edge re;
        // Status of the edge creation.
        bool success;
        boost::tie(re, success) =
          boost::edge(boost::target(ne, g), boost::source(ne, g), g);
        assert(success);

        // Map the edges to each other.
        e2e[ne] = re;
      
        // The weight of the normal edge.
        int weight = wm[ne];
      
        // Properties for the normal edge.
        e2w[ne] = weight;
        e2c[ne] = 1;
      
        // Properties for the reverse edge.
        e2w[re] = -weight;
        e2c[re] = 0;
      }

    // Set the source and destination.
    vertex s = 0, t = 1;

    boost::associative_property_map<std::map<edge, edge>> rev(e2e);
    boost::associative_property_map<std::map<edge, int>> cap(e2c);
    boost::associative_property_map<std::map<edge, int>> res(e2r);
    boost::associative_property_map<std::map<edge, int>> wgt(e2w);
    boost::successive_shortest_path_nonnegative_weights(g, s, t,
                                                        capacity_map(cap).
                                                        residual_capacity_map(res).
                                                        reverse_edge_map(rev).
                                                        weight_map(wgt));
  }

}//detail

  template <typename Graph, typename P, typename T, typename R>
  void edge_disjoint_ksp(const Graph& g,
                         typename graph_traits<Graph>::vertex_descriptor s,
                         typename graph_traits<Graph>::vertex_descriptor t,
                         const bgl_named_params<P, T, R>& params)
  {
    detail::edge_disjoint_ksp(g, s, t,
                              choose_const_pmap(get_param(params, vertex_index),
                                                g, vertex_index));
  }

#endif /* BOOST_GRAPH_EDGE_DISJOINT_KSP */
