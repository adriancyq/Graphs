/*
 * ATPGraph.h
 * Author:  Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of a graph data structure. Includes BFS for unweighted
 * searches and Dijkstra's Algorithm for weighted searches.
 */

#ifndef ATPGRAPH_H
#define ATPGRAPH_H
#include <iostream>
#include <unordered_map>
#include "ATPNode.h"
#include "Game.h"

using namespace std;

class ATPGraph
{
public:

  /*
   * Store each player in a hashmap, to ensure that each player node stores
   * unique information. The key is the player name, and the value is the
   * associated node.
   */
   unordered_map<string, ATPNode *> players;

  /*
   * Store each game in a vector, so that we can delete the game objects by
   * iterating through this.
   */
  vector<Game *> games;
  
  /*
   * Constructor function for the graph.
   */
  ATPGraph(void);

  /*
   * Populate the hashtable of players and each player's adjacency.
   *
   * Parameters:
   * filename: input filename
   *
   * Returns:
   * True if load is successful, false otherwise.
   */
  bool loadFromFile(string filename);

  /*
   * Find the shortest path between two nodes in a graph with a breadth first
   * search.
   *
   * Parameters:
   * player1: The name of the node we want to start the search at.
   * player2: the name of the node we are looking for.
   *
   * Returns:
   * True if there exists a connection between player1 and player2.
   */
  bool breadthFirstSearch(string player1, string player2);

  /*
   * Write to file who will win. TODO DONT FORGET TO RESET
   *
   * Parameters:
   * player1: name of start node.
   * player2: name of end node.
   * output: Stream to write out to.
   */
  void determineWinner(string player1, string player2, ofstream & output);

  /*
   * Destructor function for ATPGraph.
   */
  ~ATPGraph();

  /*
   * Reset all the nodes in the graph for a new search.
   */
   void reset();
};


#endif // ATPGRAPH_H
