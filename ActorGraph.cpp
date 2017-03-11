/*
 * ActorGraph.cpp
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"
#include "ActorNode.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

/*
 * Populate the movies and actors hashtables in the graph. Return the
 * earliest year in the dataset, or -1 if unable to read from the filestream.
 */
int ActorGraph::loadFromFile(string in_filename, bool use_weighted_edges) {

  // Initialize the file stream
  ifstream infile(in_filename);

  // Skip the first line, since it is a header
  bool have_header = false;

  // Keep track of the earliest year available
  int minYear = 2015;

  // keep reading lines until the end of file is reached
  while (infile) {
      string s;

      // get the next line
      if (!getline( infile, s )) break;

      // Skip the header
      if (!have_header) {
          have_header = true;
          continue;
      }

      // Parse the string
      istringstream ss( s );
      vector <string> record;

      while (ss) {
          string next;

          // Get the next string before hitting a tab character and put it
          // in 'next'
          if (!getline( ss, next, '\t' )) break;

          record.push_back( next );
      }

      // We should have exactly 3 columns: name, movie, year
      if (record.size() != 3) { continue; }

      string actor_name(record[0]);
      string movie_title(record[1]);
      int movie_year = stoi(record[2]);

      // Check if this movie is made earlier than current minYear
      if (movie_year < minYear) {
        minYear = movie_year;
      }

      // Create a unique identifier for the movie
      string movie_id = movie_title + "#@" + to_string(movie_year);

      // Check if the actor has not been encountered yet in the hashmap
      if (actors.find(actor_name) == actors.end()) {
        ActorNode * new_actor = new ActorNode(actor_name);
        actors.insert({actor_name, new_actor});
      }

      // Check if the movie has not been encountered yet in the hashmap
      Movie * curr_movie;
      int weight = 1;
      if (movies.find(movie_id) == movies.end()) {

        // Create a new movie with the correct weight
        if (use_weighted_edges) { weight = 2015 - movie_year + 1; }
        curr_movie = new Movie(movie_year, weight, movie_id, movie_title);

        // Insert into hashmap containing movies
        movies.insert({movie_id, curr_movie});
      }

      // Already have a record of the current movie
      else {
        curr_movie = movies[movie_id];
      }

      // Add the actor to the cast of the movie
      movies[movie_id]->cast.push_back(actor_name);

      // Check if the actor has been involved in a movie in the given year
      ActorNode * curr_actor = actors[actor_name];
      if (curr_actor->movie.find(movie_year) == curr_actor->movie.end()) {

        // Create a new queue to hold movies of the given year for that actor
        queue<Movie *> movieQueue;
        curr_actor->movie.insert({movie_year, movieQueue});
      }

      // Add movie to list of movies actor is involved in
      curr_actor->movie[movie_year].push(curr_movie);
  }

  if (!infile.eof()) {
      cerr << "Failed to read " << in_filename << "!\n";
      return -1;
  }
  infile.close();

  // Print number of actors and movies
  cout << "Number of actors: " << actors.size() << endl;
  cout << "Number of movies: " << movies.size() << endl;
  return minYear;
}

/*
 * Populate adjacency lists with movies made in the given year. Assume that
 * there exists at least one movie made in the given year.
 */
void ActorGraph::connectInYear(int year) {

  // Iterate through each actor
  for (auto actor: actors) {
    ActorNode * curr_actor = actor.second;

    // Go through all movies each actor was in for the given year
    Movie * curr_movie;
    queue<Movie *> movieQueue = curr_actor->movie[year];
    while(movieQueue.size() > 0) {

      // Grab the next movie
      curr_movie = movieQueue.front();
      movieQueue.pop();

      // Go through the cast of each movie
      int size = curr_movie->cast.size();
      for (int i = 0; i < size; i++) {

        // Add a new node to the current actor's adjacency list if the node
        // doesn't already exist AND the current cast member isn't the current
        // actor.
        if (curr_actor->adjacent.find(actors[curr_movie->cast[i]]) ==
            curr_actor->adjacent.end() && actors[curr_movie->cast[i]] !=
            curr_actor) {
          curr_actor->adjacent.insert({actors[curr_movie->cast[i]], curr_movie});
        }
      }
    }
  }
}

/*
 * Populate the adjacency list for each node, in order of year. Pass in
 * which year we want to start populating.
 */
void ActorGraph::createGraph(int minYear) {

  // Fill each adjacency list by year
  for (int year = minYear; year < 2016; year++) {
    connectInYear(year);
  }
}

/*
 * Reset the prev and dist fields in all te nodes in the tree.
 */
void ActorGraph::reset()
{
  for (auto actor = actors.begin(); actor != actors.end(); actor++) {
    actor->second->prev = NULL;
    actor->second->dist = numeric_limits<int>::max();
  }
  return;
}
