/*
 * ActorGraph.h
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
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
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);

  /*
   * Create the graph after reading in the information.
   */
  void createGraph();

  /*
   * Destructor function for ActorGraph.
   */
  ~ActorGraph();
};


#endif // ACTORGRAPH_H
