/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
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

        // Check if an entry for the actor does not exist
        if (actors.find(actor_name) == actors.end()) {
          ActorNode * actor = new ActorNode(actor_name);
          actors.insert({actor_name, actor});
        }

        // Check if the movie does not exist
        Movie * movie;
        auto search = movies.find(movie_id);
        if (search == movies.end()) {
          movie = new Movie(movie_title, movie_year);
          movies.insert({movie_id, movie});
        }

        // Already have records of movie
        else {
          movie = search.second;
        }

        // Add actor to movie cast
        movie->cast.push_back(actors.find(actor_name).second);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}


ActorGraph::createGraph() {
  ActorNode * p1;
  ActorNode * p2;

  // Iterate through each movie in the hashtable
  for (Movie * movie: movies) {

    // Go through all the cast members in the movies and make connections
    for (int i = 0; i < movie->cast.size(); i++) {
      for (int j = i + 1; j < movie->cast.size(); j++) {

        // Get the two people we need to make connections to
        p1 = movie->cast[i];
        p2 = movie->cast[j];

        // Link the two people
        ActorEdge * edge1 = new ActorEdge(p1, p2, movie, 2015 - movie->year);
        ActorEdge * edge2 = new ActorEdge(p2, p1, movie, 2015 - movie->year);

        // Add to respective adjacency lists
        p1->adjacent.push_back(edge1);
        p2->adjacent.push_back(edge2);
      }
    }
  }
}
