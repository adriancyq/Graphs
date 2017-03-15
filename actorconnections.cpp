/*
 * actorconnections.cpp
 * Author: Jonathan Chiu, Adrian Cordova
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Determine after which year two actors became connected using either a
 * breadth first search with a graph structure of a find operation with a
 * disjoint set structure.
 */

#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include "ActorGraph.h"
#include "UnionFind.h"

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
  bool useUfind = (algorithm.compare(string("ufind"))) ? false: true;

  // Range of years
  int minYear = 2015;
  int maxYear = 2015;

  // Initialize the graph or a disjoint set data structure
  UnionFind * disjointSet;
  ActorGraph * graph;
  if (useUfind) {
    cerr << "Use UnionFind." << endl;
    disjointSet = new UnionFind();
    minYear = disjointSet->loadFromFile(movieCasts);
    cerr << "Min year: " << minYear << endl;
  }
  else {
    cerr << "Use Graphs." << endl;
    graph = new ActorGraph();
    minYear = graph->loadFromFile(movieCasts, false);
    cerr << "Min year: " << minYear << endl;
  }

  // Initialize the input/output file stream
  cerr << "Opening File I/O streams." << endl;
  ifstream infile(testPairs);
  ofstream outfile(outputFile);
  outfile << "Actor1\tActor2\tYear" << endl;

  // Keep reading lines until end of file is reached
  bool have_header = false;
  while (infile) {
  	string s;

  	// Get the next line
  	if (!getline( infile, s)) break;

    // Skip the header
  	if (!have_header){
  		have_header = true;
  		continue;
  	}

  	// Parse the string
  	istringstream ss( s );
  	vector <string> record;

  	while (ss) {
  		string next;

  		// Get the next string before hitting a tab char and put it in 'next'
  		if (!getline( ss, next, '\t' )) break;
  		record.push_back( next );
  	}


  	//We should have exactly 2 columns: starting actor and ending actor
  	if (record.size() != 2) { continue; }
  	string actor1(record[0]);
  	string actor2(record[1]);

    // Go through each year
    bool connected = false;
    for (int year = minYear; year <= maxYear; year++) {

      // Find operation for a disjoint set
      if (useUfind) {
        disjointSet->unionInYear(year);
        connected = disjointSet->find(actor1, actor2);
      }

      // Breadth first search for a graph
      else {
        graph->connectInYear(year);
        connected = graph->breadthFirstSearch(actor1, actor2);

        // Reset the nodes for next search
        graph->reset();
      }


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

    // Clear the up trees in the disjoint set
    if (useUfind) {
      disjointSet->clear();
    }

    // Clear the adjacency lists
    else {
      graph->clear();
    }
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
