/*
 * ActorGraph.h
 * Author:  Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
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
   * Store each movie in a hashmap, where the key is the movie title + year,
   * and the value is the ActorEdge object.
   */
  unordered_map<string, Movie *> movies;

  /*
   * Store each actor in a hashmap, where the key is the actor name and the
   * value is the ActorNode object.
   */
   unordered_map<string, ActorNode *> actors;

  ActorGraph(void);

  /*
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 +
   * (2015 - movie_year), otherwise all edge weights will be 1
   *
   * Return true if file was loaded sucessfully, false otherwise
   */
  int loadFromFile(string in_filename, bool use_weighted_edges);

  bool breadthFirstSearch(string actor1, string actor2);

  void clear();

  void outputPath(string actor1, string actor2, ofstream & output);
  /*
   * Create the graph after reading in the information.
   */
  void createGraph(int minYear);

  void connectInYear(int year);

  /*
   * Destructor function for ActorGraph.
   */
  ~ActorGraph() {

    // Delete all the actors
    for (auto actor = actors.begin(); actor != actors.end(); actor++) {
      delete actor->second;
    }

    // Delete all the movies
    for (auto movie = movies.begin(); movie != movies.end(); movie++) {
      delete movie->second;
    }
  }

  /*
   * Reset the dist ad prev fields in all the nodes in the tree.
   */
   void reset();
};


#endif // ACTORGRAPH_H
