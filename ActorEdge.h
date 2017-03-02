/*
 * Movie.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to create the objects for the actor edges.
 */
#include <vector>
#include "ActorNode.h"

using namespace std;

struct Movie {
  string title;
  int year;
  vector<ActorNode *> cast;
  string id;

  Movie(string name, int yr): title(name), year(yr), cast({}) {
    id = "[" + title + "#@" + to_string(year) + "]";
  }
}

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
   * The associated movie for this edge.
   */
  Movie * movie;
  /*
   * The weight, calculated as (2015 - movie_year).
   */
  int weight;

  ActorEdge(ActorNode * src, ActorNode * dst, Movie * mov, int wt):
    source(src), dest(dst),movie(mov), weight(wt) {}
}
