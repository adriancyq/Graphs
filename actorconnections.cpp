/*
 * actorconnections.cpp
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Determine after which year two actors became connected.
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
    cerr << "Wrong number of arguments." << endl;
    return -1;
  }

  // Grab user specified files
  string movieCasts = argv[1];
  string testPairs = argv[2];
  string outputFile = argv[3];
  string algorithm = argv[4];

  // Use ufind by default
  bool useUfind = (algorithm.compare(string("ufind"))) ? true: false;

  // Initialize the graph and read in actor/movie pairs
  ActorGraph * graph = new ActorGraph();
  int minYear = graph->loadFromFile(movieCasts, false);
  int maxYear = 2015;

  // Initialize the input file stream
  ifstream infile(testPairs);

  // Initialize the output file stream
  ofstream outfile(outputFile);
  outfile << "Actor1\tActor2\tYear" << endl;

  bool have_header = false;

  //keep reading lines until end of file is reached
  while (infile) {
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


  	//We should have exactly 2 columns: starting actor and ending actor
  	if (record.size() != 2) {continue;}
  	string actor1(record[0]);
  	string actor2(record[1]);

    // Go through each year and do a BFS
    bool connected = false;
    for (int year = minYear; year <= maxYear; year++) {
      graph->connectInYear(year);
      connected = graph->breadthFirstSearch(actor1, actor2);

      // Reset the nodes for next search
      graph->reset();
      
      // Found a connection
      if (connected) {
        outfile << actor1 << "\t" << actor2 << "\t" << year << "\n";
        break;
      }
    }

    // Gone through all years and could not find a connection
    if (!connected) {
      outfile << actor1 << "\t" << actor2 << "\t" << 9999 << "\n";
    }

    // Clear the adjacency lists so we can do a fresh search
    graph->clear();
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << testPairs << "!\n";
    infile.close();
    outfile.close();
    return -1;
  }

  infile.close();
  outfile.close();
  return 0;
}
