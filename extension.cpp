/*
 * actorconnections.cpp
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 3/12/17
 *
 * Extension.cpp which traverses a graph of ATP data 
 * with players as the nodes and the edges being whether
 * or not they have played each edge will be weighted with the 
 * first serve percentage of the winner and the program will output
 * which player from the 2 user inputed players will have the better
 * probability of winning a match based on first serve percentage
 */


#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include "ATPGraph.h"
using namespace std;

int main(int argc, char ** argv) {

  // Check that user passed in 5 arguments
  if (argc != 6) {
    cerr << "Wrong number of arguments: arg1 player1 arg2 player2 arg3 w or u arg4 input file arg5 outputfile" << endl;
    cout << argc << endl;
    return -1;
  }

  // Grab user specified files
  string player1 = argv[1];
  string player2 = argv[2];
  string useWeighted = argv[3];
  bool weightedEdges = (useWeighted.compare(string("w"))) ? false : true;
  string inputFile = argv[4];
  string outputFile = argv[5];

  // Initialize the graph and read in Player/Serve Percentage pairs
  ATPGraph * graph = new ATPGraph();
  int servePerc = graph->loadFromFile(player1, weightedEdges);

  // Create the adjacency lists for each actor node
  graph->createGraph(servePerc);

  // Initialize the input file stream
  ifstream infile(inputFile);

  // Initialize the output file stream
  ofstream outfile(outputFile);
  outfile << "(Player1)--[servePerc# Percentage]-->(Player2)--..." << endl;

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


  	// We should have exactly 2 columns: starting actor and ending actor
  	if (record.size() != 2) {continue;}
  	string player1(record[0]);
  	string player2(record[1]);

    // If we are using weighted edges, use Dijkstra's algorithm
    if (weightedEdges) {
      graph->weightedSearch(player1, player2);
    }

    // If we are using unweighted edges, use a BFS
    else {
      graph->breadthFirstSearch(player1, player2);
    }

    // Output results of search
    graph->outputPath(player1, player2, outfile);
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << inputFile << "!\n";
    infile.close();
    outfile.close();
    return -1;
  }

  infile.close();
  outfile.close();
  return 0;
}