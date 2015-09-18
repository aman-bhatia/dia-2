#ifndef FEATURE_H
#define FEATURE_H

#define Coord pair<int,int>
#define RED Scalar(0,0,255)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define WHITE Scalar(255,255,255)
#define BLACK Scalar(0,0,0)
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
void triangulate();

#endif // FEATURE_H
