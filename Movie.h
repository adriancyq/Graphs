/*
 * Movie.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * Implementation of the Movie that connects two actor nodes in the graph.
 */
#ifndef MOVIE_H
#define MOVIE_H

#include <vector>

using namespace std;

class Movie
{
public:

  /*
   * List of actors involved in the movie.
   */
  vector<string> cast;

  /*
   * Year that the movie was released.
   */
  int year;

  /*
   * Weight of the edge in the graph. If unweighted, it is 1. If weighted, it
   * is 2015 - year + 1.
   */
  int weight;

  /*
   * A unique identifier for the movie.
   */
  string id;

  /*
   * Movie title.
   */
  string title;

  /*
   * Constructor function for the Movie.
   */
  Movie(int year, int weight, string id, string title): year(year),
    weight(weight), id(id), title(title) {}
};

#endif
