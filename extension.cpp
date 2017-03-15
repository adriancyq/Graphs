/*
 * extension.cpp
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
#include "Game.h"
#include "ATPNode.h"
using namespace std;

int main(int argc, char ** argv) {

  // The program requires the following:
  // 1. The dataset
  // 2. pairs of players
  // 3. Name of the output file
  if (argc != 4) {
    cerr << "3 arguments required, but received " << argc - 1 << "." << endl;
    return -1;
  }

  // Grab user specified files
  string dataset = argv[1];
  string testPairs = argv[2];
  string outputFile = argv[3];


  // Initialize the graph and read in Player/Serve Percentage pairs
  ATPGraph * graph = new ATPGraph();
  if (!graph->loadFromFile(dataset)) {
    cerr << "Unable to read from " << dataset << "." << endl;
  }

  // Initialize the input/output file stream
  ifstream infile(testPairs);
  ofstream outfile(outputFile);

  // Write header to output file
  outfile << "(Winner)\t(Loser)\t(Bias)\t#(Players In Between)..." << endl;

  //keep reading lines until end of file is reached
  bool haveHeader = false;
  while (infile) {
  	string s;

  	// Get the next line
  	if (!getline( infile, s)) break;

    // Skip the header in the input file
  	if (!haveHeader){
  		haveHeader = true;
  		continue;
  	}

  	// Parse the string
  	istringstream ss( s );
  	vector <string> record;
  	while (ss) {
  		string next;

  		//Get the next string before hitting a comma and put it in 'next'
  		if (!getline( ss, next, ',' )) break;
  		record.push_back( next );
  	}


  	// We should have exactly 2 columns for each row: players 1 and 2
  	if (record.size() != 2) {continue;}
    string player1 = record[0];
    string player2 = record[1];

    // Find the shortest path between the two players and output who will win
    if (graph->breadthFirstSearch(player1, player2)) {
      graph->determineWinner(player1, player2, outfile);
    }

    // No connection between two players
    else {
      outfile << player1 << "\t" << player2 << "\t-1\tN/A" << endl;
    }

  }

  // Unable to read test pairs
  if (!infile.eof()) {
    cerr << "Failed to read " << testPairs << "!" << endl;
    infile.close();
    outfile.close();
    return -1;
  }

  // Close filestreams
  infile.close();
  outfile.close();
  return 0;
}
