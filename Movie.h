/*
 * Movie.h
 * Author: Jonathan Chiu (A12113428), Adrian Cordova (A12010305)
 * Date:   CSE 100 Winter 2017 2/28/17
 *
 * This file is meant to exist as a way to creates the movie objects.
 */
#ifndef MOVIE_H
#define MOVIE_H

#include <vector>

using namespace std;

class Movie
{
public:
  vector<string> cast;
  int year;
  int weight;
  string id;
  string title;

  Movie(int year, int weight, string id, string title): year(year), weight(weight), id(id), title(title) {}
};

#endif
