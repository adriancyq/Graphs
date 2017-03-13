/*
 * ActorNode.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to create the objects for the player nodes.
 */

#ifndef ATPNODE_HPP
#define ATPNODE_HPP

#include <limits>
#include <unordered_map>
#include <queue>
#include "Games.h"
using namespace std;

class ATPNode
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
  ATPNode * prev;

  /*
   * A boolean indicating whether the dist an prev fields contain the final
   * best values for this vertex, or not. Used for Dijkstra's algorithm.
   */
  bool done;

  /*
   * Adjacency list implemented as a hashtable. Each entry contains
   * an adjacent node and its value is the latest game they are associated
   * with.
   */
  unordered_map<ATPNode *, Games *> adjacent;

  /*
   * All games the players has played a role in, organized by servPerc in a hashtable.
   */
  unordered_map<int, queue<Games  *>> games;

  /*
   * Name of the player.
   */
  string name;

  /*
   * Constructs an ATPNode instance.
   */
  ATPNode(string name):
      name(name), adjacent({}), dist(numeric_limits<int>::max()),
      prev(NULL), done(false) {}
};

#endif // ATPNODE_HPP
