/*
 * ActorGraph.h
 * Author:  Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of a graph data structure. Includes BFS for unweighted
 * searches and Dijkstra's Algorithm for weighted searches.
 */

 #ifndef ACTORGRAPH_H
 #define ACTORGRAPH_H

#include <iostream>
#include <unordered_map>
#include "ActorNode.h"
#include "Movie.h"

using namespace std;

class ActorGraph
{
public:

  /*
   * Store each movie in a hashmap, where the key is a unique movie ID (movie
   * title and year), and the value is a pointer to the Movie object.
   */
  unordered_map<string, Movie *> movies;

  /*
   * Store each actor in a hashmap, where the key is the actor name and the
   * value is a pointer to the ActorNode object.
   */
   unordered_map<string, ActorNode *> actors;

  /*
   * Constructor function for the graph.
   */
  ActorGraph(void);

  /*
   * Populate the hashtable of movies and actors from a tab-delimited file of
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
   * actor1: The name of the node we want to start the search at.
   * actor2: the name of the node we are looking for.
   *
   * Returns:
   * True if there exists a connection between actor1 and actor2.
   */
  bool breadthFirstSearch(string actor1, string actor2);

  /*
   * Implementation of Dijkstra's algorithm to search a graph structure with
   * weighted edges.
   *
   * Parameters:
   * actor1: The name of the node we want to start the search at.
   * actor2: the name of the node we are looking for.
   *
   * Returns:
   * True if there exists a connection between actor1 and actor2.
   */
   bool weightedSearch(string actor1, string actor2);
   
  /*
   * Disconnect all the nodes in the graph; remove all nodes from all
   * adjacency lists.
   */
  void clear();

  /*
   * Write the path defined by a breadth first search or Dijkstra's algorithm.
   *
   * Parameters:
   * actor1: name of start node.
   * actor2: name of end node.
   * output: Stream to write out to.
   */
  void outputPath(string actor1, string actor2, ofstream & output);

  /*
   * Create the graph after reading in the information.
   *
   * Parameters:
   * minYear: the year of the earliest released movie.
   */
  void createGraph(int minYear);

  /*
   * Populate adjacency lists with movies released in the given year.
   *
   * Parameters:
   * year: The year of movies we want to populate the graph with.
   */
  void connectInYear(int year);

  /*
   * Destructor function for ActorGraph.
   */
  ~ActorGraph();

  /*
   * Reset all the nodes in the graph for a new search.
   */
   void reset();
};


#endif // ACTORGRAPH_H
