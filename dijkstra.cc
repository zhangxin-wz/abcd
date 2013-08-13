#include "dijkstra.hpp"
#include <queue>
#include <utility>
#include <boost/iterator/counting_iterator.hpp>

using namespace boost;
using namespace std;

MSCPI
dijkstra(const Graph &g, Vertex src, Vertex dst, int p, const SSC &ssc)
{
  MSCPI r;

  // The null edge iterator.
  graph_traits<const Graph>::edge_iterator nei = edges(g).second;

  // We put here the information that allows us to process the source
  // node in the loop below.  We say that we reach the source node
  // with cost 0 on the subcarriers passed in the ssc argument along
  // the null edge.  The null edge signals the beginning of the path.
  r[src].insert(CPI(0, PI(*nei, ssc)));

  // The priority queue of the pairs, where the first element in the
  // pair tells the cost of reaching vertex that is the second element
  // in the pair.  We need not only the vertex, but also the edge to
  // know what subcarriers we exactly need.
  priority_queue<pair<int, pair<Vertex, Edge> > > q;

  // We reach the source vertex with cost 0 along the null edge.
  q.push(make_pair(0, make_pair(src, *nei)));

  while(!q.empty())
    {
      // Here we process vertex v.  
      pair<int, pair<Vertex, Edge> > pqe = q.top();
      q.pop();
      int c = pqe.first;
      Vertex v = pqe.second.first;
      Edge e = pqe.second.second;

      // The SCPI for node v in r.
      SCPI &scpi = r[v];

      // Security check: make sure there is the info in r on reaching
      // vertex v along edge e with cost c.  Is there a chance that
      // this info is missing?  For instance, a better result was
      // found before and was introduced there?
      //assert(includes(scpi, cost, edge));
      
      // We reached vertex v with cost c.  Now we should know with
      // what subcarriers we reached this node.

      // Itereate over the out edges of the vertex.
      //      graph_traits<Graph>::out_edge_iterator ei, eei;
      //      for(tie(ei, eei) = out_edges(v, g); ei != eei; ++ei)
      //	{
      //	}
    }

  return r;
}
