/*
 * ATPGraph.cpp
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of the necessary methods for extension
 * and basic graph setup for the ATPGraph.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ATPGraph.h"
#include "ATPNode.h"

using namespace std;

/*
 * Constructor function for the graph.
 */
ATPGraph::ATPGraph(void) {}

/*
 * Populate the hashtable of players and each player's adjacency.
 *
 * Parameters:
 * filename: input filename
 *
 * Returns:
 * True if load is successful, false otherwise.
 */
bool ATPGraph::loadFromFile(string filename)
{
  ifstream infile(filename);

  // Keep reading lines until the end of file is reached
  bool haveHeader = false;
  while (infile) {
    string s;

    // Get the next line
    if (!getline(infile, s)) break;

    // Skip the header
    if (!haveHeader) {
      haveHeader = true;
      continue;
    }

    // Parse the string and store components (name, movie title, year)
    istringstream ss( s );
    vector <string> record;
    while (ss) {
      string next;

      // Get the next string before hitting a tab char and put it in 'next'
      if (!getline(ss, next,',')) break;
      record.push_back( next );
    }

    // We should have exactly 3 columns: name, movie title, year
    if (record.size() != 49) { continue; }
    string tournamentName = record[1];
    string winnerName = record[10];
    string loserName = record[20];
    int winnerFirstServe = stoi(string(record[35]));
    int loserFirstServe = stoi(string(record[44]));

    // Check if the player has not been encountered yet in the hashmap
    if (players.find(winnerName) == players.end()) {
      ATPNode * newPlayer = new ATPNode(winnerName);
      players.insert({winnerName, newPlayer});
    }

    if (players.find(loserName) == players.end()) {
      ATPNode * newPlayer = new ATPNode(loserName);
      players.insert({loserName, newPlayer});
    }

    // Create two new Game objects
    Game * winnerGame = new Game(loserName, winnerFirstServe, tournamentName);
    Game * loserGame = new Game(winnerName, -loserFirstServe, tournamentName);
    games.push_back(winnerGame);
    games.push_back(loserGame);

    // Add to each player's adjacency list
    players[winnerName]->adjacent.insert({players[loserName], winnerGame});
    players[loserName]->adjacent.insert({players[winnerName], loserGame});
  }

  // Unable to read from input file
  if (!infile.eof()) {
      cerr << "Failed to read " << filename << "!\n";
      return false;
  }
  infile.close();

  // Print number of players
  cout << "Number of players: " << players.size() << "." << endl;
  return true;
}

/*
 * Find the shortest path between two nodes in a graph with a breadth first
 * search.
 *
 * Parameters:
 * player1: The name of the node we want to start the search at.
 * player2: the name of the node we are looking for.
 *
 * Returns:
 * True if there exists a connection between player1 and player2.
 */
bool ATPGraph::breadthFirstSearch(string player1, string player2)
{
  //Check that the players exist
  if (players.find(player1) == players.end() ||
      players.find(player2) == players.end()) {
    return false;
  }

  // Grab the start and end nodes
  ATPNode * start = players[player1];
  ATPNode * end = players[player2];

  // Reset the graph
  reset();

  // Initialize the queue and add starting node
  queue<ATPNode *> toExplore;
  start->dist = 0;
  toExplore.push(start);

  // Do a BFS
  while (!toExplore.empty()) {

    // Dequeue the head
    ATPNode * head = toExplore.front();
    toExplore.pop();

    // Check if we found the node we're looking for
    if (head->name == end->name) { return true; }

    // Explore each of the neighbors
    for (auto node = head->adjacent.begin(); node != head->adjacent.end();
        node++) {
      ATPNode * curr_node = node->first;

      // Check if distance is infinity (not visited)
      if (curr_node->dist == numeric_limits<int>::max()) {
        curr_node->dist = head->dist + 1;
        curr_node->prev = head;
        toExplore.push(curr_node);
      }
    }
  }

  // Gone through the entire graph and could not find a connection
  return false;
}

/*
 * Write to file who will win.
 *
 * Parameters:
 * player1: name of start node.
 * player2: name of end node.
 * output: Stream to write out to.
 */
void ATPGraph::determineWinner(string player1, string player2,
    ofstream & output) {

  // Grab the start and end nodes
  ATPNode * start = players[player1];
  ATPNode * end = players[player2];

  // Initialize a vector to keep track of nodes in the path
  vector<ATPNode *> path;

  // Traverse the path from end to start
  while (start != end) {

    // End if BFS was unsuccessful and there does not exists a path from
    // start to end nodes
    if (!end) {return; }

    // Traverse path from end to start
    path.push_back(end);
    end = end->prev;
  }

  // Add the starting node to the last position in the vector
  path.push_back(start);

  // Determine if the bias is positive or negative
  int bias = 0;
  for (int i = path.size() - 1; i > 0; i--) {
    bias += path[i]->adjacent[path[i - 1]]->bias;
  }

  // First player wins if bias is positive
  if (bias >= 0) {
    output << player1 << "\t" << player2 << "\t" << bias << "\t" <<
      path.size() - 2 << endl;
  }

  // Second player wins if bias is negative
  else {
    bias *= -1;
    output << player2 << "\t" << player1 << "\t" << bias << "\t" <<
      path.size() - 2 << endl;
  }
}

/*
 * Destructor function for ActorGraph.
 */
ATPGraph::~ATPGraph()
{

  // Delete all the players
  for (auto player = players.begin(); player != players.end(); player++) {
    delete player->second;
  }


}

/*
 * Reset all the nodes in the graph for a new search.
 */
 void ATPGraph::reset() {
   for (auto player = players.begin(); player != players.end(); ++player) {
      player->second->dist = numeric_limits<int>::max();
      player->second->prev = NULL;
   }
 }
