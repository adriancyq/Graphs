#include <vector>
#include "ActorEdge.h"

using namespace std;

class ActorNode
{

private:

  /*
   * Cost of the best path discovered so far from the start vertex to this
   * vertex. Used for Dijkstra's algorithm.
   */
  int dist;

  /*
   * The previous node on that best path. Used for Dijkstra's algorithm.
   */
  ActorNode * prev;

  /*
   * A boolean indicating whether the dist an prev fields contain the final
   * best values for this vertex, or not. Used for Dijkstra's algorithm.
   */
  bool done;

public:

  /*
   * Adjacency list containing edges where the current node is the source.
   */
  vector<ActorEdge *> adjacent;

  /*
   * Name of the actor.
   */
  string name;

  /*
   * Constructs an ActorNode instance.
   */
  ActorNode(string name):
      name(name), adjacent({}), distance(numeric_limits<int>::min()),
      prev(NULL), done(false) {}
}
