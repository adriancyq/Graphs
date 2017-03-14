/*
 * ActorNode.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Declaration of the node class in a graph data structure. Each node
 * represents a player that played in an ATP tournament.
 */

#ifndef ATPNODE_HPP
#define ATPNODE_HPP

#include <limits>
#include <unordered_map>
#include <queue>
#include "Game.h"
using namespace std;

class ATPNode
{
public:

  /*
   * Name of the player, used as an unique identifier.
   */
  string name;

  /*
   * Cost of the best path discovered so far from the start vertex to this
   * vertex.
   */
  int dist;

  /*
   * The previous node on that best path. Used to output the shortest path
   * after a breadth first search.
   */
  ATPNode * prev;

  /*
   * Adjacency list containing games (as edges) that the player played in.
   */
  vector<Game *> adjacent;

  /*
   * Constructs an ATPNode instance.
   */
  ATPNode(string name): name(name), dist(numeric_limits<int>::max()),
    prev(NULL) {}
};

#endif // ATPNODE_HPP
