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
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}


void ActorGraph::createGraph() {
  return;
}
