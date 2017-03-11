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
  //graph->loadFromFile(movieCasts, weightedEdges);

  // // Create the adjacency lists for each actor node
  // graph->createGraph();
  //
  // // Initialize the input file stream
  // ifstream infile(testPairs);
  //
  // // Initialize the output file stream
  // ofstream outfile(outputFile);
  // outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;
  //
  // bool have_header = false;
  //
  // //keep reading lines until end of file is reached
  // while (infile) {
  // 	string s;
  //
  // 	//get the next line
  // 	if (!getline( infile, s)) break;
  //
  // 	if (!have_header){
  // 		have_header = true;
  // 		continue;
  // 	}
  //
  // 	// Parse the string
  // 	istringstream ss( s );
  // 	vector <string> record;
  //
  // 	while (ss) {
  // 		string next;
  //
  // 		//Get the next string before hitting a tab character
  // 		//and put it in 'next'
  // 		if (!getline( ss, next, '\t' )) break;
  //
  // 		record.push_back( next );
  // 	}
  //
  //
  // 	//We should have exactly 2 columns: starting actor and ending actor
  // 	if (record.size() != 2) {continue;}
  // 	string actor1(record[0]);
  // 	string actor2(record[1]);
  //
  //   // Find shortest path from actor 1 to actor 2
  //   breadthFirstSearch(actor1, actor2, *graph);
  //   outputPath(actor1, actor2, outfile, *graph);
  // }
  //
  // if (!infile.eof()) {
  //   cerr << "Failed to read " << testPairs << "!\n";
  //   infile.close();
  //   outfile.close();
  //   return -1;
  // }
  //
  // infile.close();
  // outfile.close();
  // return 0;
}
