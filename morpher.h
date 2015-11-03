#ifndef MORPHER_H
#define MORPHER_H

#include "mainwindow.h"
#include "photo.h"

extern bool warp;

class Morpher
{
public:
  int TIME;    // in second
  double FPS;    // frames per second

	Morpher();
	~Morpher();
	Point3f getBarycentricCoord(Point2f p, vector<Point2f> &triangle);
	vector< Coord > getFeaturePoints(vector<Photo> photos, vector<float> weights);
	Mat morph_photos(vector<Photo> &photos, vector<float> weights);
	Mat warp_photos(Photo orig_img,vector<Photo> &photos, vector<float> weights);
};

#endif // MORPHER_H
