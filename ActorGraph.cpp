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

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

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

        // Create a unique identifier for the movie
        string movie_id = "[" + movie_title + "#@" + to_string(movie_year) + "]";

        // Check if the actor has not been encountered yet in the hashmap
        if (actors.find(actor_name) == actors.end()) {
          ActorNode * new_actor = new ActorNode(actor_name);
          actors.insert({actor_name, new_actor});
        }

        // Check if the movie has not been encountered yet in the hashmap
        Movie * curr_movie;
        if (movies.find(movie_id) == movies.end()) {

          // If weighted edge, use 2015 - movie year + 1
          if (use_weighted_edges) {
            curr_movie = new Movie(movie_year, 2015 - movie_year + 1, movie_id,
              movie_title);
          }

          // If unweighted edge, use 1 for all edges
          else {
            curr_movie = new Movie(movie_year, 1, movie_id, movie_title);
          }

          // Insert into hashmap containing movies
          movies.insert({movie_id, curr_movie});
        }

        // Already have a record of the current movie
        else {
          curr_movie = movies[movie_id];
        }

        // Add the actor to the cast of the movie
        movies[movie_id]->cast.push_back(actor_name);

        // Add movie to list of movies actor is involved in
        actors[actor_name]->movie.push(curr_movie);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    // Print number of actors and movies
    cout << "Number of actors: " << actors.size() << endl;
    cout << "Number of movies: " << movies.size() << endl;
    return true;
}

/*
 * Populate the adjacency list for each node, using actors and movies
 * read in from loadFromFile.
 */
void ActorGraph::createGraph() {

  // Iterate through each actor
  for (auto actor: actors) {
    Movie * curr_movie;
    ActorNode * curr_actor = actor.second;

    // Go through all movies each actor was in
    while (curr_actor->movie.size() > 0) {

      // Grab the next most recent movie the actor was in
      curr_movie = curr_actor->movie.top();
      curr_actor->movie.pop();

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
 * Reset the prev and dist fields in all te nodes in the tree.
 */
void ActorGraph::reset()
{
  for (auto actor = actors.begin(); actor != actors.end(); actor++) {
    actor->second->prev = NULLL;
    actor->second->dist = numeric_limits<int>::max();
  }
  return;
}
