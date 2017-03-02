/*
 * pathfinder.cpp
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to find the shortest path
 * from one actor to another actor through shared movies.
 */

 #include <sstream>
 #include <cstdlib>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <queue>
 #include <string>
 #include "ActorGraph.h"

using namespace std;

int main(int argc, char ** argv) {

  // Check that user passed in 4 arguments
  if (argc != 5) {
    cerr << "Wrong number of arguments" << endl;
    return -1;
  }

  // Grab user specified files
  string movieCasts = argv[1];
  bool weightedEdges = (argv[2] == "w") ? true: false;
  string testPairs = argv[3];
  string outputFile = argv[4];

  // Initialize the graph and read in actor/movie pairs
  ActorGraph * graph = new ActorGraph();
  graph->loadFromFile(movieCasts, weightedEdges);

  // Create the adjacency lists for each actor node
  graph->createGraph();

  // TODO Read in testPairs
  //initialize the file stream
  ifstream infile(testPairs);

  bool have_header = false;

  //keep reading linues until end of file is reached
  while (infile){
  	string s;

  	//get the next line
  	if (!getline( infile, s)) break;

  	if (!have_header){
  		have_header = true;
  		continue;
  	}

  	// Parse the string 
  	istringstream ss( s );
  	vector <string> record;

  	while (ss) {
  		string next;

  		//Get the next string before hitting a tab character
  		//and put it in 'next'
  		if (!getline( ss, next, '\t' )) break;

  		record.push_back( next );
  	}
  	//We should have exactly 3 columns: name, movie, year
  	if (record.size() != 3) {continue;}

  	string actor_name(record[0]);
  	string movie_title(record[1]);
  	int movie_year = stoi(record[2]);

  if (!infile.eof()) {
        cerr << "Failed to read " << testPairs << "!\n";
        return false;
    }
    infile.close();

  // TODO Use BFS to find links
  
  return 0;
  }
}
