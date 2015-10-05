#ifndef MORPHER_H
#define MORPHER_H

#include "mainwindow.h"
#include "photo.h"

class Morpher
{
public:
  int TIME;    // in second
  double FPS;    // frames per second
  bool make_video;
  string window_name;

  Morpher();
  void morph_photos(Photo &photo1, Photo &photo2, string name);
  void merge_parts(string name);
};

#endif // MORPHER_H
