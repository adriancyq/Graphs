/*
 * UnionFind.h
 * Author:  Jonathan Chiu, Adrian Cordova
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of a disjoint set data structure for actorconnections.
 */

 #ifndef UNIONFIND_H
 #define UNIONFIND_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "Movie.h"

using namespace std;

class UnionFind
{
public:

  /*
   * Store movies by year. Each year will have a different hashtable to store
   * movie Ids and Movie objects.
   */
  unordered_map<int, unordered_map<string, Movie *>> movieYears;

  /*
   * Store each actor in a hashmap, where the key is the actor name and the
   * value is the corresponding index in the vector.
   */
   unordered_map<string, int> actors;

  /*
   * Disjoint set data structure implemented with an array.
   */
  vector<int> actorForest;

  /*
   * Counts the number of nodes in the forest rooted at index i.
   */
  vector<int> size;

  UnionFind(void) {}

  /*
   * Populate the movie and actor hash tables, as well as the cast for each
   * movie.
   *
   * Parameters:
   * in_filename: input filename
   *
   * Returns:
   * The year of the earliest movie made if file was loaded sucessfully, or
   * -1 otherwise.
   */
  int loadFromFile(string filename);

  /*
   * Performs the union operation.
   *
   * Parameters:
   * actor1: index of first actor
   * actor2: index of second actor
   */
   void makeUnion(int actor1, int actor2);

  /*
   * Gets the root of the up tree.
   *
   * Parameters:
   * Index of the current actor.
   *
   * Returns:
   * The index of the root.
   */
  int getRoot(int actor);

  /*
   * Find if two actors are connected. Compare their roots.
   *
   * Parameters:
   * actor1: name of first actor.
   * actor2: name of second actor.
   */
   bool find(string actor1, string actor2);

  /*
   * Make unions for all movies in a given year.
   *
   * Parameters:
   * year: The year of movies we want to populate the disjoint set with.
   */
  void unionInYear(int year);

  /*
   * Reset all up trees in the disjoint set structure.
   */
   void clear();
};

#endif // UNIONFIND_H
