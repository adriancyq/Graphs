/*
 * Movie.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Implementation of the Games that connects two player nodes in the graph.
 */
#ifndef GAMES_H
#define GAMES_H

#include <vector>

using namespace std;

class Games
{
public:

  /*
   * List of players involved in the game.
   */
  vector<string> opponents;


  /*
   * Weight of the edge in the graph. If unweighted, it is 1. If weighted, it
   * is the first serve percentage.
   */
  int weight;

  /*
   * A unique identifier for the game.
   */
  string id;


  /*
   * Constructor function for the game.
   */
  Games(int year, int weight, string id):
    weight(weight), id(id) {}
};

#endif
