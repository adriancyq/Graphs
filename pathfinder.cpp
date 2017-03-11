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

class nodeComparer{
  public:
    bool operator() (ActorNode*& first, ActorNode*& next){
      return (first->dist) > (next->dist);
    }
};

void Dijkstra(string &actor1, string &actor2, ActorGraph &graph) {
  string startActor = actor1;
  string nextActor = actor2;
  priority_queue<ActorNode*, vector<ActorNode*>, nodeComparer> actorQ;
  ActorNode* front;
  int current_distance;


  graph.actors[startActor]->dist = 0;
  actorQ.push(graph.actors[startActor]);

  while (!actorQ.empty()){
    front = actorQ.top();
    actorQ.pop();

    if (front->done){
      continue;
    }
    else{
      front->done = true;
      for (auto theOne = front->adjacent.begin(); theOne != front->adjacent.end(); ++theOne) {
        current_distance = front->dist + (theOne->second->weight);

        if (current_distance < theOne->first->dist){
          theOne->first->dist = current_distance;
          theOne->first->prev = front;
          actorQ.push(theOne->first);
        }
      }
    }
  }
}

int main(int argc, char ** argv) {

  // Check that user passed in 4 arguments
  if (argc != 5) {
    cerr << "Wrong number of arguments" << endl;
    return -1;
  }

  string second = argv[2];
  string weight = "w";

  // Grab user specified files
  string movieCasts = argv[1];
  bool weightedEdges = (second.compare(weight)) ? true: false;
  string testPairs = argv[3];
  string outputFile = argv[4];

  // Initialize the graph and read in actor/movie pairs
  ActorGraph * graph = new ActorGraph();
  int minYear = graph->loadFromFile(movieCasts, weightedEdges);

  // Create the adjacency lists for each actor node
  graph->createGraph(minYear);

  // Initialize the input file stream
  ifstream infile(testPairs);

  // Initialize the output file stream
  ofstream outfile(outputFile);
  outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;

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
  	string actor1(record[0]);
  	string actor2(record[1]);

    // If we are using weighted edges, use Dijkstra's algorithm
    if (weightedEdges) {
      Dijkstra(actor1, actor2, *graph);
    }

    // If we are using unweighted edges, use a BFS
    else {
      graph->breadthFirstSearch(actor1, actor2);
    }

    // Output results of search
    graph->outputPath(actor1, actor2, outfile);
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
