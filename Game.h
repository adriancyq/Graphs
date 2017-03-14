/*
 * Game.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Implementation of a Game object that acts as the edge between two nodes in
 * a graph. Required for extension.cpp.
 */
#ifndef GAME_H
#define GAME_H

#include <vector>

using namespace std;

class Game
{
public:

  /*
   * Name of opponent.
   */
  string opponent;


  /*
   * If the player won, the biases is + (number of first serves made). If lost,
   * the bias is - (number of first serves made).
   */
  int bias;

  /*
   * A unique identifier for the game.
   */
  string id;


  /*
   * Constructor function for the game.
   */
  Game(string opponent, int bias, string id): opponent(opponent), bias(bias),
    id(id) {}
};

#endif
