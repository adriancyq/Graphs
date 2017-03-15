/*
 * ActorGraph.cpp
 * Author: Jonathan Chiu, Adrian Cordova
 * Date: CSE 100 Winter 2017 2/28/17
 *
 * Implementation of the necessary methods for pathfinder, actorconnections
 * and basic graph setup for the ActorGraph.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"
#include "ActorNode.h"

using namespace std;

/*
 * Constructor function for the graph.
 */
ActorGraph::ActorGraph(void) {}

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
int ActorGraph::loadFromFile(string in_filename, bool use_weighted_edges)
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
    if (actors.find(actor_name) == actors.end()) {
      ActorNode * new_actor = new ActorNode(actor_name);
      actors.insert({actor_name, new_actor});
    }

    // Check if the movie has not been encountered yet in the hashmap
    Movie * curr_movie;
    int weight = 1;
    if (movies.find(movie_id) == movies.end()) {

      // Create a new movie with the correct weight
      if (use_weighted_edges) { weight = (2015 - movie_year) + 1; }
      curr_movie = new Movie(movie_year, weight, movie_id, movie_title);

      // Insert into hashmap containing movies
      movies.insert({movie_id, curr_movie});
    }

    // Already have a record of the current movie
    else {
      curr_movie = movies[movie_id];
    }

    // Add the actor to the cast of the movie
    movies[movie_id]->cast.push_back(actor_name);

    // Check if the actor has been involved in a movie in the given year
    ActorNode * curr_actor = actors[actor_name];
    if (curr_actor->movie.find(movie_year) == curr_actor->movie.end()) {

      // Create a new queue to hold movies of the given year for that actor
      queue<Movie *> movieQueue;
      curr_actor->movie.insert({movie_year, movieQueue});
    }

    // Add movie to list of movies actor is involved in
    curr_actor->movie[movie_year].push(curr_movie);
  }

  if (!infile.eof()) {
      cerr << "Failed to read " << in_filename << "!\n";
      return -1;
  }
  infile.close();

  // Print number of actors and movies
  cout << "Number of actors: " << actors.size() << endl;
  cout << "Number of movies: " << movies.size() << endl;
  return minYear;
}

/*
 * Populate adjacency lists with movies made in the given year. Assume that
 * there exists at least one movie made in the given year.
 *
 * Parameters:
 * year: The year of movies we want to populate the graph with.
 */
void ActorGraph::connectInYear(int year)
{
  // Iterate through each actor
  for (auto actor: actors) {
    ActorNode * curr_actor = actor.second;

    // Go through all movies each actor was in for the given year
    Movie * curr_movie;
    queue<Movie *> movieQueue = curr_actor->movie[year];
    while(movieQueue.size() > 0) {

      // Grab the next movie
      curr_movie = movieQueue.front();
      movieQueue.pop();

      // Go through the cast of each movie
      int size = curr_movie->cast.size();
      for (int i = 0; i < size; i++) {

        // Add a new node to the current actor's adjacency list if the node
        // doesn't already exist AND the current cast member isn't the current
        // actor.
        if (curr_actor->adjacent.find(actors[curr_movie->cast[i]]) ==
            curr_actor->adjacent.end()) {
          if (actors[curr_movie->cast[i]] != curr_actor) {
            curr_actor->adjacent.insert({actors[curr_movie->cast[i]],
                curr_movie});
          }

        }

        // We have already encountered the same actor in the adjacency list,
        // but we need to check if the current movie is more recent. If so,
        // we replace the values in case we use a weighted graph.
        else {

          // Compare the weight of the existing movie to the weight of the
          // new movie
          if (curr_actor->adjacent.find(actors[curr_movie->cast[i]])->
              second->weight > curr_movie->weight) {
            curr_actor->adjacent.find(actors[curr_movie->cast[i]])->second =
                curr_movie;
          }

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
void ActorGraph::createGraph(int minYear)
{

  // Fill each adjacency list by year
  for (int year = minYear; year < 2016; year++) {
    connectInYear(year);
  }
}

/*
 * Reset all the nodes in the graph for a new breadth first search or weighted
 * search.
 */
void ActorGraph::reset()
{
  for (auto actor = actors.begin(); actor != actors.end(); actor++) {
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
void ActorGraph::clear()
{
  for (auto actor = actors.begin(); actor != actors.end(); actor++) {
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
bool ActorGraph::breadthFirstSearch(string actor1, string actor2)
{

  //Check that the players exist
  if (actors.find(actor1) == actors.end() ||
      actors.find(actor2) == actors.end()) {
    return false;
  }

  // Grab the start and end nodes
  ActorNode * start = actors[actor1];
  ActorNode * end = actors[actor2];

  // Reset the graph
  reset();

  // Initialize the queue and add starting node
  queue<ActorNode *> toExplore;
  start->dist = 0;
  toExplore.push(start);

  // Do a BFS
  while (!toExplore.empty()) {

    // Dequeue the head
    ActorNode * head = toExplore.front();
    toExplore.pop();

    // Check if we found the node we're looking for
    if (head->name == end->name) { return true; }

    // Explore each of the neighbors
    for (auto node = head->adjacent.begin(); node != head->adjacent.end();
        node++) {
      ActorNode * curr_node = node->first;

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
void ActorGraph::outputPath(string actor1, string actor2, ofstream & output) {

  // Grab the start and end nodes
  ActorNode * start = actors[actor1];
  ActorNode * end = actors[actor2];

  // Initialize a vector to keep track of nodes in the path
  vector<ActorNode *> path;

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
ActorGraph::~ActorGraph()
{

  // Delete all the actors
  for (auto actor = actors.begin(); actor != actors.end(); actor++) {
    delete actor->second;
  }

  // Delete all the movies
  for (auto movie = movies.begin(); movie != movies.end(); movie++) {
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
bool ActorGraph::weightedSearch(string actor1, string actor2)
{
  //Check that the players exist
  if (actors.find(actor1) == actors.end() ||
      actors.find(actor2) == actors.end()) {
    return false;
  }

  // Reset the graph for a new search
  reset();

  // Initialize the priority queue of nodes to explore
  priority_queue<ActorNode *, vector<ActorNode *>, nodeComparator> toExplore;

  // Enqueue the start node to begin the search
  actors[actor1]->dist = 0;
  toExplore.push(actors[actor1]);

  // Explore all nodes
  while (!toExplore.empty()) {

    // Dequeue the next node in the shortest path
    ActorNode * front = toExplore.top();
    toExplore.pop();

    // Check if we found the end node
    if (front->name.compare(actor2) == 0) { return true; }

    // Node's min path hasn't been discovered yet
    if (!front->done) {
      front->done = true;

      // Go through all the neighbors
      for (auto node = front->adjacent.begin(); node != front->adjacent.end();
          ++node) {
        ActorNode * curr_node = node->first;  // Next neighbor
        Movie * curr_movie = node->second;    // Movie connecting two actors

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
