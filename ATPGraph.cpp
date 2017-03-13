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
 * Populate the hashtable of movies and actors from a tab-delimited file of
 * actor-movie relationships. Also populates the list of movies that each
 * actor is involved in.
 *
 * Parameters:
 * in_filename: input filename
 * use_weighted_edges: if true, compute edge weights as 1 +
 * (2015 - movie_year), otherwise all edge weights will be 1.
 *
 * Returns:
 * The year of the earliest movie made if file was loaded sucessfully, or
 * -1 otherwise.
 */
int ATPGraph::loadFromFile(string in_filename, bool use_weighted_edges)
{
  ifstream infile(in_filename);                     // Input filestream
  bool have_header = false;                         // Skip the first line
  int minYear = 2015;                               // Earliest released movie

  // Keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // Get the next line
    if (!getline( infile, s )) break;

    // Skip the header
    if (!have_header) {
        have_header = true;
        continue;
    }

    // Parse the string and store components (name, movie title, year)
    istringstream ss( s );
    vector <string> record;
    while (ss) {
      string next;

      // Get the next string before hitting a tab char and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }

    // We should have exactly 3 columns: name, movie title, year
    if (record.size() != 3) { continue; }
    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // Keep track of date of earliest released movie
    if (movie_year < minYear) {
      minYear = movie_year;
    }

    // Create a unique identifier for the movie
    string movie_id = movie_title + "#@" + to_string(movie_year);

    // Check if the actor has not been encountered yet in the hashmap
    if (players.find(actor_name) == players.end()) {
      ATPNode * new_actor = new ATPNode(actor_name);
      players.insert({actor_name, new_actor});
    }

    // Check if the movie has not been encountered yet in the hashmap
    Games * curr_movie;
    int weight = 1;
    if (games.find(movie_id) == games.end()) {

      // Create a new movie with the correct weight
      if (use_weighted_edges) { weight = (2015 - movie_year) + 1; }
      curr_movie = new Games(movie_year, weight, movie_id);

      // Insert into hashmap containing movies
      games.insert({movie_id, curr_movie});
    }

    // Already have a record of the current movie
    else {
      curr_movie = games[movie_id];
    }

    // Add the actor to the cast of the movie
    games[movie_id]->opponents.push_back(actor_name);

    // Check if the actor has been involved in a movie in the given year
    ATPNode * curr_actor = players[actor_name];
    if (curr_actor->games.find(movie_year) == curr_actor->games.end()) {

      // Create a new queue to hold movies of the given year for that actor
      queue<Games *> movieQueue;
      curr_actor->games.insert({movie_year, movieQueue});
    }

    // Add movie to list of movies actor is involved in
    curr_actor->games[movie_year].push(curr_movie);
  }

  if (!infile.eof()) {
      cerr << "Failed to read " << in_filename << "!\n";
      return -1;
  }
  infile.close();

  // Print number of actors and movies
  cout << "Number of actors: " << players.size() << endl;
  cout << "Number of movies: " << games.size() << endl;
  return minYear;
}

/*
 * Populate adjacency lists with movies made in the given year. Assume that
 * there exists at least one movie made in the given year.
 *
 * Parameters:
 * year: The year of movies we want to populate the graph with.
 */
void ATPGraph::connectInServPerc(int servPerc)
{

  // Iterate through each actor
  for (auto actor: players) {
    ATPNode * curr_actor = actor.second;

    // Go through all movies each actor was in for the given year
    Games * curr_movie;
    queue<Games *> movieQueue = curr_actor->games[servPerc];
    while(movieQueue.size() > 0) {

      // Grab the next movie
      curr_movie = movieQueue.front();
      movieQueue.pop();

      // Go through the cast of each movie
      int size = curr_movie->opponents.size();
      for (int i = 0; i < size; i++) {

        // Add a new node to the current actor's adjacency list if the node
        // doesn't already exist AND the current cast member isn't the current
        // actor.
        if (curr_actor->adjacent.find(players[curr_movie->opponents[i]]) ==
            curr_actor->adjacent.end() && players[curr_movie->opponents[i]] !=
            curr_actor) {
          curr_actor->adjacent.insert({players[curr_movie->opponents[i]], curr_movie});
        }
      }
    }
  }
}

/*
 * Populate the adjacency list for each node, in order of year. Pass in the
 * year of the earliest movie.
 *
 * Parameters:
 * minYear: the year of the earliest released movie.
 */
void ATPGraph::createGraph(int minYear)
{

  // Fill each adjacency list by year
  for (int year = minYear; year < 2016; year++) {
    connectInServPerc(year);
  }
}

/*
 * Reset all the nodes in the graph for a new breadth first search or weighted
 * search.
 */
void ATPGraph::reset()
{
  for (auto actor = players.begin(); actor != players.end(); actor++) {
    actor->second->prev = NULL;
    actor->second->dist = numeric_limits<int>::max();
    actor->second->done = false;
  }
  return;
}

/*
 * Disconnect all the nodes in the graph; remove all nodes from all
 * adjacency lists.
 */
void ATPGraph::clear()
{
  for (auto actor = players.begin(); actor != players.end(); actor++) {
    actor->second->adjacent.clear();
  }
}

/*
 * Breadth first search on the graph structure, assuming the edges are
 * unweighted.
 *
 * Parameters:
 * actor1: The name of the node we want to start the search at.
 * actor2: the name of the node we are looking for.
 *
 * Returns:
 * True if there exists a connection between actor1 and actor2.
 */
bool ATPGraph::breadthFirstSearch(string player1, string player2)
{

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
    for (auto node = head->adjacent.begin(); node != head->adjacent.end(); node++) {
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
 * Write the path defined by a breadth first search or Dijkstra's algorithm.
 *
 * Parameters:
 * actor1: name of start node.
 * actor2: name of end node.
 * output: Stream to write out to.
 */
void ATPGraph::outputPath(string player1, string player2, ofstream & output) {

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

  // Output the path from start to second to end (or else there will be an
  // extra -->)
  for (int i = path.size() - 1; i > 0; i--) {
    output << '(' << path[i]->name << ")--[" <<
      path[i]->adjacent[path[i - 1]]->id << "]-->";
  }

  // Output the last node in the path
  output <<'('<< path[0]->name << ')'<<endl;
  return;
}

/*
 * Destructor function for ActorGraph.
 */
ATPGraph::~ATPGraph()
{

  // Delete all the actors
  for (auto actor = players.begin(); actor != players.end(); actor++) {
    delete actor->second;
  }

  // Delete all the movies
  for (auto movie = games.begin(); movie != games.end(); movie++) {
    delete movie->second;
  }
}

/*
 * Implementation of Dijkstra's algorithm to search a graph structure with
 * weighted edges.
 *
 * Parameters:
 * actor1: The name of the node we want to start the search at.
 * actor2: the name of the node we are looking for.
 *
 * Returns:
 * True if there exists a connection between actor1 and actor2.
 */
bool ATPGraph::weightedSearch(string player1, string player2)
{
  // Grab the start and end nodes
  ATPNode * start = players[player1];
  ATPNode * end = players[player2];

  // Reset the graph for a new search
  reset();

  // Initialize the priority queue of nodes to explore
  priority_queue<ATPNode *, vector<ATPNode *>, nodeComparator> toExplore;

  // Enqueue the start node to begin the search
  start->dist = 0;
  toExplore.push(start);

  // Explore all nodes
  while (!toExplore.empty()) {

    // Dequeue the next node in the shortest path
    ATPNode * front = toExplore.top();
    toExplore.pop();

    // Check if we found the end node
    if (front->name == end->name) { return true; }

    // Node's min path hasn't been discovered yet
    if (!front->done) {
      front->done = true;

      // Go through all the neighbors
      for (auto node = front->adjacent.begin(); node != front->adjacent.end(); ++node) {
        ATPNode * curr_node = node->first;  // Next neighbor
        Games * curr_movie = node->second;    // Movie connecting two actors

        // Distance to neighbor from current node
        int total_dist = front->dist + (curr_movie->weight);

        // Smaller weight path has been found
        if (total_dist < curr_node->dist) {
          curr_node->dist = total_dist;
          curr_node->prev = front;
          toExplore.push(curr_node);
        }
      }
    }
  }

  // Searched the entire graph and could not find end node from start
  return false;
}
