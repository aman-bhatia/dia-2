#ifndef FEATURE_H
#define FEATURE_H

#define Coord pair<int,int>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

extern Mat img;
extern string window_name;
extern vector<string> image_path;
extern vector<Mat> image;
extern vector< vector< Coord> > feature_points;

void draw(int event, int x, int y, int flags, void* param);
void setFeatures();

#endif // FEATURE_H
