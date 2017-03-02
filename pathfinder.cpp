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

void breadthFirstSearch(string actor1, string actor2, ActorGraph & graph) {
  ActorNode * start = graph.actors[actor1];
  ActorNode * end = graph.actors[actor2];

  // Initialize the queue and add starting node
  queue<ActorNode *> toExplore;
  start->dist = 0;
  toExplore.push(start);

  // Do a BFS
  while (!toExplore.empty()) {

    // Dequeue the head
    ActorNode * head = toExplore.top();
    toExplore.pop();

    // Check if we found the node we're looking for
    if (head->name == end->name) { return; }

    // Explore each of the neighbors
    for (auto node = head->adjacent.begin(); node != head->adjacent.end(); node++) {
      ActorNode * curr_node = node.first;

      // Check if distance is infinity (not visited)
      if (curr_node->dist == numeric_limits<int>::max()) {
        curr_node->dist = head->dist + 1;
        curr_node->prev = head;
        toExplore.push(curr_node);
      }
    }
  }
}

void outputPath(string actor1, string actor2, ofstream & output, ActorGraph & graph)
{
  // Grab the start and end nodes
  ActorNode * start = graph.actors[actor1];
  ActorNode * end = graph.actors[actor2];

  // Initialize a vector to keep track of nodes in the path
  vector<ActorNode *> path;

  // Traverse the path from end to start
  while (start != end) {
    path.push_back(end);
    end = end->prev;
  }

  // Output the path from start to second to end (or else there will be an
  // extra -->)
  for (int i = path.size() - 1; i > 0; i--) {
    output << '(' << path[i]->name << ")--[" <<
      path[i]->adjacent[path[i - 1]]->id << "]-->";
  }

  // Output the last node in the path
  output <<'('<< path[0]->name << ')'<<endl;

  // Reset the graph
  graph.reset();
  return;
}

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
   if (!graph->loadFromFile(movieCasts, weightedEdges)) {
     cerr << "Unable to load actor/movie pairs from file." << endl;
   }

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
