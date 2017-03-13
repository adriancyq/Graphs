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
#include "Games.h"

using namespace std;

/*
 * Comparator class for the priority queue, to be used for Dijkstra's
 * algorithm. Nodes with smaller distance have higher priority.
 */
class nodeComparator
{
  public:
    bool operator() (ATPNode *& first, ATPNode *& second)
    {
      return (first->dist) > (second->dist);
    }
};

class ATPGraph
{
public:

  /*
   * Store each game in a hashmap, where the key is a unique game ID (game number
  * and servPerc), and the value is a pointer to the Game object.
   */
  unordered_map<string, Games *> games;

  /*
   * Store each player in a hashmap, where the key is the player name and the
   * value is a pointer to the ATPNode object.
   */
   unordered_map<string, ATPNode *> players;

  /*
   * Constructor function for the graph.
   */
  ATPGraph(void);

  /*
   * Populate the hashtable of Games and players from a tab-delimited file of
   * actor-movie relationships.
   *
   * Parameters:
   * in_filename: input filename
   * use_weighted_edges: if true, compute edge weights as 1 +
   * (2015 - movie_year), otherwise all edge weights will be 1.
   *
   * Returns:
   * The year of the earliest movie made if file was loaded sucessfully, or
   * -1 otherwise.
   */
  int loadFromFile(string in_filename, bool use_weighted_edges);

  /*
   * Breadth first search on the graph structure, assuming the edges are
   * unweighted.
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
   * Implementation of Dijkstra's algorithm to search a graph structure with
   * weighted edges.
   *
   * Parameters:
   * player1: The name of the node we want to start the search at.
   * player2: the name of the node we are looking for.
   *
   * Returns:
   * True if there exists a connection between player1 and player2.
   */
   bool weightedSearch(string player1, string player2);

  /*
   * Disconnect all the nodes in the graph; remove all nodes from all
   * adjacency lists.
   */
  void clear();

  /*
   * Write the path defined by a breadth first search or Dijkstra's algorithm.
   *
   * Parameters:
   * player1: name of start node.
   * player2: name of end node.
   * output: Stream to write out to.
   */
  void outputPath(string player1, string player2, ofstream & output);

  /*
   * Create the graph after reading in the information.
   *
   * Parameters:
   * servePerc: the first serve percentage of the player.
   */
  void createGraph(int servePerc);

  /*
   * Populate adjacency lists with movies released in the given year.
   *
   * Parameters:
   * year: The year of movies we want to populate the graph with.
   */
  void connectInServPerc(int servePerc);

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
