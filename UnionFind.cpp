/*
 * UnionFind.cpp
 * Author: Jonathan Chiu, Adrian Cordova
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of union and find operations for the disjoint set data
 * structure.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "UnionFind.h"

using namespace std;

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
int UnionFind::loadFromFile(string filename)
{
  ifstream infile(filename);                        // Input filestream
  bool haveHeader = false;                          // Skip the first line
  int minYear = 2015;                               // Earliest released movie
  int actorIndex = 0;                               // Index unique actors

  // Keep reading lines until EOF
  while (infile) {
    string s;

    // Get the next line
    if (!getline( infile, s )) break;

    // Skip the header
    if (!haveHeader) {
      haveHeader = true;
      continue;
    }

    // Parse the string and store components (name, movie title, year)
    istringstream ss(s);
    vector <string> record;
    while (ss) {
      string next;

      // Get the next string before hitting a tab char and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    // We should have exactly 3 columns: name, movie title, year
    if (record.size() != 3) { continue; }
    string actorName(record[0]);
    string movieTitle(record[1]);
    int movieYear = stoi(record[2]);

    // Keep track of date of earliest released movie
    if (movieYear < minYear) {
      minYear = movieYear;
    }

    // Create a unique identifier for the movie
    string movieId = movieTitle + "#@" + to_string(movieYear);

    // Check if the actor has not been encountered yet
    if (actors.find(actorName) == actors.end()) {

      // Actor will be at index 'actorIndex' in the array
      actors.insert({actorName, actorIndex});
      actorForest.push_back(actors[actorName]);
      size.push_back(1);
      actorIndex++;
    }

    // Check if we have not encountered a movie in the specified year
    if (movieYears.find(movieYear) == movieYears.end()) {

      // Create a new hashtable for that year
      unordered_map<string, Movie *> movies;
      movieYears.insert({movieYear, movies});
    }

    // Check if the movie has not been encountered yet
    if (movieYears[movieYear].find(movieId) == movieYears[movieYear].end()) {
      Movie * newMovie = new Movie(movieYear, 1, movieId, movieTitle);
      movieYears[movieYear].insert({movieId, newMovie});
    }

    //Add the actor to the cast of the movie
    movieYears[movieYear][movieId]->cast.push_back(actorName);
  }

  // Return -1 on unsuccessful filestream ops
  if (!infile.eof()) {
      cerr << "Failed to read " << filename << "!\n";
      return -1;
  }
  infile.close();

  // Print number of actors
  cout << "Number of actors: " << actors.size() << endl;

  // Return the year of the earliest movie on record
  return minYear;
}

/*
 * Performs a weighted union operation.
 *
 * Parameters:
 * actor1: index of first actor
 * actor2: index of second actor
 */
void UnionFind::makeUnion(int actor1, int actor2)
{
  // Grab the roots of the two actors
  int root1 = getRoot(actor1);
  int root2 = getRoot(actor2);

  // Actor 2's subtree is bigger, merge smaller tree to bigger tree
  if (size[root1] < size[root2]) {
    actorForest[root1] = root2;
    size[root2] += size[root1];
  }

  // Actor 1's subtree is bigger,
  else {
    actorForest[root2] = root1;
    size[root1] += size[root2];
  }
}

/*
 * Gets the root of the up tree.
 *
 * Parameters:
 * Index of the current actor.
 *
 * Returns:
 * The index of the root.
 */
int UnionFind::getRoot(int actor)
{
  int current = actor;
  while (current != actorForest[current]) {

    // Traverse up the tree
    current = actorForest[current];

    // Path compression
    actorForest[current] = actorForest[actorForest[current]];
  }

  // Return index of the root
  return current;
}

/*
 * Find if two actors are connected. Compare their roots.
 *
 * Parameters:
 * actor1: name of first actor.
 * actor2: name of second actor.
 */
 bool UnionFind::find(string actor1, string actor2)
 {
  int actorIndex1 = actors[actor1];
  int actorIndex2 = actors[actor2];

  return getRoot(actorIndex1) == getRoot(actorIndex2);
 }

/*
 * Make unions for all movies in a given year.
 *
 * Parameters:
 * year: The year of movies we want to populate the disjoint set with.
 */
void UnionFind::unionInYear(int year)
{
  // Get the collection of movies for the given year
  unordered_map<string, Movie *> movieInYear = movieYears[year];

  // Iterate through all the movies
  for (auto current = movieInYear.begin(); current != movieInYear.end(); current++) {
    Movie * movie = current->second;

    // Iterate through all the casts for each movie
    for (int i = 0; i < movie->cast.size(); i++) {
      for (int j = i + 1; j < movie->cast.size(); j++) {

        // Grab the index for the ith and jth actors
        int actor1 = actors[movie->cast[i]];
        int actor2 = actors[movie->cast[j]];

        // Make union between two actors
        makeUnion(actor1, actor2);
      }
    }
  }
}

/*
 * Reset all up trees in the disjoint set structure.
 */
void UnionFind::clear()
{

  // Make all indices unique
  for (int i = 0; i < actorForest.size(); i++) {
    actorForest[i] = i;
  }

  // Make all sizes 1
  for (int i = 0; i < size.size(); i++) {
    size[i] = 1;
  }
}
