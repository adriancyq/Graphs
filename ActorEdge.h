#include <vector>
#include "ActorNode.h"

using namespace std;

class ActorEdge
{
public:

  /*
   * The node that this edge connects from.
   */
  ActorNode * source;

  /*
   * The node that this edge connects to.
   */
  ActorNode * dest;

  /*
   * The name of the movie.
   */
  string name;

  /*
   * The year this was movie was released.
   */
  int year;

  ActorEdge(ActorNode * source, ActorNode * dest, string name, int year):
    source(source), dest(dest), name(name), year(year) {}
}
