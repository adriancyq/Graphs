/*
 * ActorNode.h
 * Author: Jonathan Chiu, Adrian Cordova
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to create the objects for the actor nodes.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <limits>
#include <unordered_map>
#include <queue>
#include "Movie.h"
using namespace std;

class ActorNode
{
public:

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

  /*
   * Adjacency list implemented as a hashtable. Each entry contains
   * an adjacent node and its value is the latest movie they are associated
   * with.
   */
  unordered_map<ActorNode *, Movie *> adjacent;

  /*
   * All movies the actor played a role in, organized by year in a hashtable.
   */
  unordered_map<int, queue<Movie *>> movie;

  /*
   * Name of the actor.
   */
  string name;

  /*
   * Constructs an ActorNode instance.
   */
  ActorNode(string name):
      name(name), adjacent({}), dist(numeric_limits<int>::max()),
      prev(NULL), done(false) {}
};

#endif // ACTORNODE_HPP
