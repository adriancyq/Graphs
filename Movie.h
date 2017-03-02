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
